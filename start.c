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
		do{
			printw("%c", htravel->data);
			htravel = htravel->next;
		}while(htravel != NULL);
		vtravel = vtravel->next;
	}while(vtravel != NULL);
	
}

int main(int argc, char *argv[]) {
	char ch;
	
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
					if(hlength(hl) > x)
						move(y, ++x);
					refresh();
					break;
				case 'q':
					break;
				case '\n':
					hinsert(&hl, ch, x);
					if(x == 0){
						vinsert(&vl, &hl, y);
					}
					/*if(x < hlength(hl)){
					//	x++;
						hbreak(&hl, x);
						vinsert(&vl, &hl, y+1);
						

					}*/
					init_hlist(&hl);
					y++;
					x = 0;
					move(y, x);	
					break;
					
				default : 
					hinsert(&hl, ch, x);
					if(hlength(hl) == 1)
						vinsert(&vl, &hl, y);
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
		}
		endwin();
	}
	return 0;
}