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
int x, y, ROWS, COLS;
int arr_len = 1;
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

int highlight(char *info, char *file_name, int ROWS) {
	int y_prev, x_prev, ch;
	getyx(stdscr, y_prev, x_prev);
	attron(A_REVERSE);
	move(ROWS - 2, 0);
	clrtoeol();
	printw( "%s %s \n", info, file_name);
	printw("Are you sure Y / N : ");
	attroff(A_REVERSE);
	printw("  ");
	ch  = getch();
	addch(ch);
	refresh();
	if(ch == 'Y')
		return 1;
	move(y_prev, x_prev);
	return 0;
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

void traverse_and_assign(vlist *vl, hlist *hl, int y){
						vnode *t;
						t = vl->top;
						for(int i=0; i < y; i++)
							t = t->next;
						t->row = hl->head;
}	

char* cut_word(vlist *vl, hlist *hl, int x, char *arr){
	hnode *start, *end ,*temp;
	int i, flag = 0;
	temp = hl->head;
	for(i = 0; i < x-1; i++){
		temp = temp->next;
	}
	start = temp;
	end = temp;
	if(start->prev != NULL){
		while(((start->prev)->data != ' ') && ((start->prev)->data != '\n')){
			start = start->prev;
			if(start->prev == NULL){
				flag = 1;
				break;
			}
			else{
				flag = 0;
				break;
			}
		}
	}
	if(end->next != NULL){
		while((end->next->data != ' ') && (end->next->data != '\n')){
			end = end->next;
			if(end->next == NULL){
				break;
			}
		}
	}

	if((start->prev->data == ' ') && (end->next->data == ' ')){
		
	}


	temp = start;
	while(temp != end){
		arr_len++;
		temp = temp->next;
	}
	//printw("%d", arr_len);
	arr = realloc(arr, arr_len);
	temp = start;
	i = 0;
	arr[i] = temp->data;
	//printw("%c", arr[i]);
	i++;
	do{
		temp = temp->next; 
		arr[i] = temp->data;
		//printw("%c", arr[i]);
		i++;
	}while(temp != end);
	
	if(flag == 0){
		start->prev->next = end->next;
		end->next->prev = start->prev;
		start->prev = NULL;
		end->next = NULL;

	hnode* now = start;
		while( now != NULL ) {
   			hnode* next = now->next;
   			free(now);
   			now = next;
		}
		free(now);
	}
	if(flag == 1){
		current->row = end->next;
		end->next->prev = NULL;
		start->prev = NULL;
		end->next = NULL;

		hnode* now = start;
		while( now != NULL ) {
   			hnode* next = now->next;
   			free(now);
   			now = next;
		}
		free(now);
	}

	return arr;
}

char* copy_word(hlist *hl, int x, char *arr){
	hnode *start, *end ,*temp;
	int i;
	temp = hl->head;
	for(i = 0; i < x-1; i++){
		temp = temp->next;
	}
	start = temp;
	end = temp;
	if(start->prev != NULL){
		while(((start->prev)->data != ' ') && ((start->prev)->data != '\n')){
			start = start->prev;
			if(start->prev == NULL){
				break;
			}
		}
	}
	if(end->next != NULL){
		while((end->next->data != ' ') && (end->next->data != '\n')){
			end = end->next;
			if(end->next == NULL){
				break;
			}
		}
	}
	temp = start;
	while(temp != end){
		arr_len++;
		temp = temp->next;
	}
	//printw("%d", arr_len);
	arr = realloc(arr, arr_len);
	temp = start;
	i = 0;
	arr[i] = temp->data;
	//printw("%c", arr[i]);
	i++;
	do{
		temp = temp->next; 
		arr[i] = temp->data;
		//printw("%c", arr[i]);
		i++;
	}while(temp != end);
	
	return arr;
}

void paste_word(vlist *vl, hlist *hl, int *x, char *arr){
	int i;
	for(i = 0; i < arr_len; i++){
		//printw("%c ", arr[i]);
		hinsert(hl, arr[i], *x);
		if(hlength(*hl) == 1){   //
						vinsert(vl, hl, y);
						//current = vl->bottom;
						traverse_and_assign(vl, hl, y);
						current = current->next;
		}
		//if(vlength(*vl) == 1)
		//				current = vl->bottom;
										

		(*x) = (*x) + 1;
	}
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
void quit(vlist *vl) {
	vnode *vtravel;
	hnode *htravel;

	vtravel = vl->top;
	if(vtravel == NULL) {
		return ;	
	}
	if(vtravel == vl->bottom){
		htravel = vtravel->row;
		if(htravel == NULL) {
			free(vtravel);
			return ;	
		}


		hnode* now = htravel;
		while( now != NULL ) {
   			hnode* next = now->next;
   			free(now);
   			now = next;
		}
		free(vtravel);
	}
	while(vtravel != NULL){
		htravel = vtravel->row;
		hnode* now = htravel;
			while( now != NULL ) {
   				hnode* next = now->next;
   				free(now);
   				now = next;
			}
		vtravel = vtravel->next;	
	}
	vtravel = vl->top;
	vnode* vnow = vtravel;
	while(vnow != NULL){
		vnode* next = vnow->next;
   		free(vnow);
   		vnow = next;
	}
}
	
int main(int argc, char *argv[]) {
	char ch, prev_ch, *arr, *c;
	
	hlist hl;	
	vlist vl;

	arr = (char*)malloc(10*sizeof(char));

	int i, j, m, flag = 0;
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
		getmaxyx(stdscr, ROWS, COLS);
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
						hl.rear = temp;
						if(hl.head == NULL) 
							move(++y, x);
						else if(hl.rear->data != '\n')
							move(++y, x);	
						else
							move(++y, --x);
					}
					break;
						
				case (char)KEY_BACKSPACE:
					if((y >= 0) && (x > 0)){
						if((x == 1)){
							hl.head->data = ' ';
							clear();
							print_list(&vl);
							move(y, x);
							refresh();
							}
						else{
							temp = hl.head;
							for(i=0;i<(x-1);i++){
								temp = temp->next;
							}
							if(x == hlength(hl)){
								if(hlength(hl) == 1){
									free(temp);
								}
								else{
									temp->prev->next = NULL;
									hl.rear = hl.rear->prev;
									free(temp); 
								}
							}
							else if(x < hlength(hl)){
								if(x == 1){
									hl.head = hl.head->next;
									current->row = hl.head;
									free(temp);
								}
								else{
									temp->next->prev = temp->prev;
									temp->prev->next = temp->next;
									free(temp);
								}
							}
							clear();
							print_list(&vl);
							move(y, --x);
							refresh();

						}	
					}
					break;	

				case (char)KEY_F(8):
					cut_word(&vl, &hl, x, arr);
					clear();
					print_list(&vl);
					refresh();
					break;

				case (char)KEY_F(3):
					copy_word(&hl, x, arr);
					break;

				case (char)KEY_F(4):
					paste_word(&vl, &hl, &x, arr);
					move(y, x);
					clear();
					print_list(&vl);
					refresh();
					break;			
				case (char)KEY_F(5):
					save_file(&vl, &hl, argv[1]);
					break;
				case (char)KEY_F(7):
					c = "Quiting the file";
					m = highlight(c, "", ROWS);
					if( m == 1)
						quit(&vl);
					clear();
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
			if((ch == (char)KEY_F(7)) && (m == 1)) {
				break;
			}
			prev_ch = ch;
		}
		endwin();
	}
	return 0;
}
