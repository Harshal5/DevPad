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

int x = 0, y = 0, ROWS, COLS;
int arr_len = 1;
vnode *current;
hnode *temp;

int exists(char *a) {				//to check whether the file exists
	FILE *fp;
	fp = fopen(a, "r");
	if(fp != NULL) 
		return 1;
	else
		return 0;
}

void save_file(vlist *vl, hlist *hl, char *file_name) {			//save the editted file

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

int highlight(char *info, char *file_name, int ROWS) {			//popup at the bottom of the screen
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


void print_prompt() {						//choice of options
	int prev_y, prev_x;
	getyx(stdscr, prev_y, prev_x);
	attron(A_BOLD);
	mvprintw(ROWS-1, COLS - 100, "F5 : To save ");
	mvprintw(ROWS-1, COLS - 80, "F7 : To Quit ");
	mvprintw(ROWS-1, COLS - 60, "F4 : To paste ");
	mvprintw(ROWS-1, COLS - 40, "F3 : To Copy Word ");
	mvprintw(ROWS-1, COLS - 20, "F8 : To Cut Word ");
	attroff(A_BOLD);
	move( prev_y, prev_x);
}


void print_loc(int y, int x) {						//current coordinates
	mvprintw(ROWS-1, 0, "x: %d y: %d ", x, y);
	move(y, x);
}

void traverse_and_assign(vlist *vl, hlist *hl, int y){        //assign hl to the row of the vl 
						vnode *t;
						t = vl->top;
						for(int i=0; i < y; i++)
							t = t->next;
						t->row = hl->head;
}	

char* cut_word(vlist *vl, hlist *hl, int x, char *arr){				//performs cut operation on the word
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



	temp = start;
	while(temp != end){
		arr_len++;
		temp = temp->next;
	}
	arr = realloc(arr, arr_len);
	temp = start;
	i = 0;
	arr[i] = temp->data;
	i++;
	do{
		temp = temp->next; 
		arr[i] = temp->data;
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

char* copy_word(hlist *hl, int x, char *arr){				//perform copy operation on the word
	
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
	arr = realloc(arr, arr_len);
	temp = start;
	i = 0;
	arr[i] = temp->data;
	i++;
	do{
		temp = temp->next; 
		arr[i] = temp->data;
		i++;
	}while(temp != end);
	
	return arr;

}

void paste_word(vlist *vl, hlist *hl, int *x, char *arr){		//perform paste operation on the word
	
	int i;
	for(i = 0; i < arr_len; i++){
		hinsert(hl, arr[i], *x);
		if(hlength(*hl) == 1){   //
					vinsert(vl, hl, y);
					traverse_and_assign(vl, hl, y);
					current = current->next;
		}
		(*x) = (*x) + 1;
	}

}

void print_list(vlist *vl){ 						//printing the whole list 
	
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
void quit(vlist *vl) {							//exit the file
	
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


void read_file(vlist *vl, hlist *hl, char *filename) {				//read data from a existing file
	
	FILE *fp;
	int ch;
	int x = 0, y = 0;
	fp = fopen(filename, "r");
	initscr();
	raw();
	noecho();
	keypad(stdscr, TRUE);
	init_hlist(hl);
	init_vlist(vl);
	ch = fgetc(fp);
	while(ch != EOF) {
		switch(ch) {
			case '\n':
				x++;
				init_hlist(hl);
				y++;
				x = 0;

			default:
				hinsert(hl, ch, x);
				if((x == 0)&&(hlength(*hl) == 1)){   //
					vinsert(vl, hl, y);
					current = vl->bottom;
				}
					x++;	
				break;
		}
		ch = fgetc(fp);
	}
	fclose(fp);

}	
	
int main(int argc, char *argv[]) {

	char ch, *arr, *c;
	
	hlist hl;	
	vlist vl;

	arr = (char*)malloc(10*sizeof(char));

	int i, m;
	if(argc == 2) {
		if(exists(argv[1]))
			read_file(&vl, &hl, argv[1]);
		else{
			initscr();
			raw();
			noecho();
			refresh();
			keypad(stdscr, TRUE);
			init_hlist(&hl);
			init_vlist(&vl);
					
		}		

		move(0, 0);
		current = vl.top;
		getyx(stdscr, y, x);
		getmaxyx(stdscr, ROWS, COLS);
		print_list(&vl);
		move(0, 0);
		refresh();
		while(1) {								//start inputs
			print_loc(y, x);
	
			ch = getch();
			//clear();
			switch(ch) {
				case (char)KEY_LEFT:
					if(x>0)
						move(y, --x);
					refresh();
					break;
				case (char)KEY_RIGHT:
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

						x = 0; 						//taking x to 1 then moving it forward
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
							for(i = 0;i < (x-1); i++){
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
						if(hlength(hl) == 1){ 					//for double \n 
							vinsert(&vl, &hl, y);
							current = current->next;
							init_hlist(&hl);
						}
						else{
							traverse_and_assign(&vl, &hl, y);
							x++;
							hbreak(&hl, x);
							vinsert(&vl, &hl, y+1);
							current = current->next;
						}
					}

					else if(x < hlength(hl)){
						hinsert(&hl, ch, x);
						x++;
						hbreak(&hl, x);
						vinsert(&vl, &hl, y+1);
						current = current->next;
					}
					else{
						hinsert(&hl, ch, x);
						
						init_hlist(&hl);
					}

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
					traverse_and_assign(&vl, &hl, y);
					if(vlength(vl) == 1)
						current = vl.bottom;
		
					clear();
					print_list(&vl);	
					move(y, ++x);
					refresh();	
					break;
			}
			refresh();
			print_prompt();	
			if((ch == (char)KEY_F(7)) && (m == 1)) {
				break;
			}
		
		}
		endwin();
	}
	return 0;
}
