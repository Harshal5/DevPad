#include<stdio.h>
#include<stdlib.h>
#include<ncurses.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include "hlist.h"
#include "vlist.h"
int x, y;

vnode *current;
hnode *temp;

void save_file(vlist *vl, hlist *hl, char *file_name) {

	FILE *fp;
	fp = fopen(file_name, "w+");
	vnode *vtravel;
	hnode *htravel;

	vtravel = vl->top;

	if(vtravel == NULL) {
		return ;	
	}
	if(vtravel == vl->bottom){
		htravel = vtravel->row;
		if(htravel == NULL) {
			return ;	
		}
		if(htravel->next == NULL){
			fputc(htravel->data, fp);
			return;
		}
		do {
			fputc(htravel->data, fp);
			htravel = htravel->next;
		}while(htravel != NULL);
		return;
	}
	do{
		htravel = vtravel->row;
		do{
			fputc(htravel->data, fp);
			htravel = htravel->next;
		}while(htravel != NULL);
		vtravel = vtravel->next;
	}while(vtravel != NULL);
	fclose(fp);
	
}

void print_prompt() {
	int prev_y, prev_x;
	getyx(stdscr, prev_y, prev_x);
	attron(A_BOLD);
	mvprintw(1, COLS - 22, "F5 : To save ");
	mvprintw(2, COLS - 22, "F7 : To Quit ");
	mvprintw(3, COLS - 22, "F3 : To replace ");
	mvprintw(4, COLS - 22, "F2 : To search ");
	mvprintw(5, COLS - 22, "DEL : To Delete line ");
	mvprintw(6, COLS - 22, "F4 + L : To Copy line ");
	mvprintw(7, COLS - 22, "F4 + W : To Copy Word ");
	mvprintw(8, COLS - 22, "F6 : To Paste ");
	mvprintw(9, COLS - 22, "F8 + W : To Cut Word ");
	mvprintw(10, COLS - 22, "F8 + L : To Cut Line ");
	mvprintw(11, COLS - 22, "ESC : To Escape ");
	attroff(A_BOLD);
	move( prev_y, prev_x);
}


void print_loc(int y, int x) {
	mvprintw(0, COLS - 22, "x: %d y: %d ", x, y);
	move(y, x);
}

void print_list(vlist *vl){
	vnode *vtravel;
	hnode *htravel;

	vtravel = vl->top;

	if(vtravel == NULL) {
		return ;	
	}
	if(vtravel == vl->bottom){
		htravel = vtravel->row;
		if(htravel == NULL) {
			return ;	
		}
		if(htravel->next == NULL){
			printw("%c", htravel->data);
			return;
		}
		do {
			printw("%c", htravel->data);
			htravel = htravel->next;
		}while(htravel != NULL);
		return;
	}
	do{
		htravel = vtravel->row;
		if(htravel == NULL)
			return;
		do{
			printw("%c", htravel->data);
			htravel = htravel->next;
		}while(htravel != NULL);
		vtravel = vtravel->next;
	}while(vtravel != NULL);
	
}

void traverse_and_assign(vlist *vl, hlist *hl, int y){
						vnode *t;
						t = vl->top;
						for(int i=0; i < y; i++)
							t = t->next;
						t->row = hl->head;
}	

int main(int argc, char *argv[]) {
	char ch, prev_ch;
	
	hlist hl;	
	vlist vl;

	int i, flag = 0;
	if(argc == 2) {
		init_hlist(&hl);
		init_vlist(&vl);
		initscr();
		raw();
		noecho();
		refresh();
		keypad(stdscr, TRUE);
		move(0, 0);
		getyx(stdscr, y, x);
		while(1) {
		print_loc(y, x);
			
			ch = getch();
			switch(ch) {
				case (char)KEY_LEFT:
					//printw("left");
					//flag = 1;
					//x--;
					//move_left(&l);
					if(x>0)
						move(y, --x);
					refresh();
					break;
				case (char)KEY_RIGHT:
					//flag = 1;
					//move_right(&l);
					if(hlength(hl) > x){
						if((vlength(vl) > y) &&((x == (hlength(hl)-1)))){

						}
						else	
							move(y, ++x);
						
					}
					refresh();
					break;
				case (char)KEY_UP:
					if((y>0)&&(current->prev != NULL)){

						x = 0; //taking x to 1 then moving it forward
						if(hlength(hl) != 0)
							current = current->prev;
						hl.head = current->row;
						temp = hl.head;
						while(((temp->next) != NULL)){
								temp = temp->next;
								x++;
						}
						move(--y, x);
						hl.rear = temp;	
					}	
					break;	
				case (char)KEY_DOWN:
					if((vlength(vl)-1) > y) {
						x = 1;
						current = current->next;
						hl.head = current->row;
						temp = hl.head;
						while(((temp->next) != NULL)){
								temp = temp->next;
								x++;
						}
						if(hl.head == NULL)
							move(++y, x);
						else
							move(++y, --x);
						hl.rear =temp;
					}
					break;
						
				case (char)KEY_BACKSPACE:
					break;		

				case 'q':
					break;
				case '\n':
					if(vl.top == NULL){
						vinsert(&vl, &hl, y);
						hinsert(&hl, ch, x);
						vl.top->row = hl.head;
						init_hlist(&hl);
						current = vl.bottom;
					}
					else if(x == 0){
						hinsert(&hl, ch, x);
						if(hlength(hl) == 1){ //forr double \n 
							vinsert(&vl, &hl, y);
							current = current->next;//
							init_hlist(&hl);
						}
						else{
							//remove later add traverse
							traverse_and_assign(&vl, &hl, y);
							x++;
							hbreak(&hl, x);
							vinsert(&vl, &hl, y+1);
							current = current->next;
						}
					}

					else if(x < hlength(hl)){
						hinsert(&hl, ch, x);
						//traverse_and_assign(&vl, &hl, y);
						x++;
						hbreak(&hl, x);
						vinsert(&vl, &hl, y+1);
						// add traverse

						current = current->next;
						//delete later
						//vl.bottom->row = hl.head;
					}
					else{
						hinsert(&hl, ch, x);
						//vinsert(&vl, &hl, y);
						init_hlist(&hl);
						//current = current->next;
					}

					//try for x<len
					
					//y++;
					//x = 0;
					
					clear();
					print_list(&vl);
					move(++y, x = 0);	
					refresh();
					break;
					
				default : 
					hinsert(&hl, ch, x);
					if((x == 0)&&(hlength(hl) == 1)){   //
						vinsert(&vl, &hl, y);
						current = vl.bottom;
					}
					// add traverse
					/*	vnode *t;
						t = vl.top;
						for(i=0;i<y;i++)
							t = t->next;
						t->row = hl.head;	
					*/traverse_and_assign(&vl, &hl, y);


					if(vlength(vl) == 1)
						current = vl.bottom;
										

				    //delete later
					//vl.bottom->row = hl.head;	
					//addch(ch);
					//x++;
					//if(x == 0)
					//	vreplace(&vl, &hl, y);
					clear();
					print_list(&vl);	
					move(y, ++x);
					refresh();	
					break;
			}
			refresh();
			//print_prompt();	
			if(ch == 'q') {
				save_file(&vl, &hl, argv[1]);
				break;
			}
			prev_ch = ch;
		}
		endwin();
	}
	return 0;
}
