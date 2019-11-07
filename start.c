#include<stdio.h>
#include<stdlib.h>
#include<ncurses.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

int x, y;

typedef struct hnode {
	char data;
	struct hnode *prev, *next;
}hnode;

typedef struct hlist {
	hnode *head, *rear;
}hlist;

typedef struct vnode {
	hnode *row;
	struct vnode *prev, *next;
}vnode;

typedef struct vlist {
	vnode *top, *bottom;
}vlist;

void init_hlist(hlist *hl) {
	hl->head = NULL;
	hl->rear = NULL;
}


int length (hlist hl) {
	hnode *temp;
	temp = hl.head;
	if(temp == NULL) {
		return 0;	
	}
	int len = 1;
	if(temp == hl.rear){
		return len;
	}
	do {
		temp = temp->next;
		len++;
	}while(temp != hl.rear);
	return len;
}

void insert (hlist *hl, char c, int pos) {
	hnode *temp, *new_hnode;
	int len, i = 0;
	len = length(*hl);
	if (pos <0 || pos > len)
		return;
	
	
	new_hnode = (hnode*)malloc(sizeof(hnode));
	new_hnode->data = c;
	if(len == 0) {
		hl->head = new_hnode;
		hl->rear = new_hnode;
		new_hnode->prev = NULL;
		new_hnode->next = NULL;	
		return;
	}	
	
	if(pos == 0) {
		new_hnode->next = hl->head;
		new_hnode->prev = NULL;
		hl->head->prev = new_hnode;
		hl->head = new_hnode;
		return;
	}
		
	if (pos == len) {
		new_hnode->prev = hl->rear;
		new_hnode->next = NULL;
		hl->rear->next = new_hnode;
		hl->rear = new_hnode;
		return;
	}
	
	
	temp = hl->head;
	for(i = 0; i < pos - 1; i++) {
		temp = temp->next;	
	}
	
	new_hnode->prev = temp;
	new_hnode->next = temp->next;
	temp->next->prev = new_hnode;
	temp->next = new_hnode;
}

void print_hlist(hlist hl) {
	hnode *temp;
	temp = hl.head;	
	if(temp == NULL) {
		return ;	
	}
	if(temp == hl.rear){
		printw("%c", temp->data);
		return;
	}
		
	do {
		printw("%c", temp->data);
		temp = temp->next;
	}while(temp != NULL);
	
}


void save_file(hlist *hl, char *file_name) {

	FILE *fp;
	fp = fopen(file_name, "w+");
	hnode* travel;
	travel = hl->head;
	while(travel != NULL) {
		fputc(travel->data, fp);
		travel = travel->next;
	}
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

int main(int argc, char *argv[]) {
	char ch;
	
	hlist hl;

	int i, flag = 0;
	if(argc == 2) {
		init_hlist(&hl);
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
					if(length(hl) > x)
						move(y, ++x);
					refresh();
					break;
				case 'q':
					break;
				/*case '\n':
					insert(&l, ch, x);
					clear();
					print_hlist(l);
					move(++y, ++x);
					refresh();
					break;	
				*/default : 
					insert(&hl, ch, x);
					//addch(ch);
					//x++;
					
					clear();
						
					print_hlist(hl);	
					move(y, ++x);
					refresh();	
					break;
			}
			refresh();
			//print_prompt();	
			if(ch == 'q') {
				save_file(&hl, argv[1]);
				break;
			}
		}
		endwin();
	}
	return 0;
}	
