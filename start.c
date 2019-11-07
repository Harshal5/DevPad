#include<stdio.h>
#include<stdlib.h>
#include<ncurses.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

int x, y;

typedef struct node {
	char data;
	struct node *prev, *next;
}node;

typedef struct list {
	node *head, *rear;
}list;

void init_list(list *l) {
	l->head = NULL;
	l->rear = NULL;
}


int length (list l) {
	node *temp;
	temp = l.head;
	if(temp == NULL) {
		return 0;	
	}
	int len = 1;
	if(temp == l.rear){
		return len;
	}
	do {
		temp = temp->next;
		len++;
	}while(temp != l.rear);
	return len;
}

void insert (list *l, char c, int pos) {
	node *temp, *new_node;
	int len, i = 0;
	len = length(*l);
	if (pos <0 || pos > len)
		return;
	
	
	new_node = (node*)malloc(sizeof(node));
	new_node->data = c;
	if(len == 0) {
		l->head = new_node;
		l->rear = new_node;
		new_node->prev = NULL;
		new_node->next = NULL;	
		return;
	}	
	
	if(pos == 0) {
		new_node->next = l->head;
		new_node->prev = NULL;
		l->head->prev = new_node;
		l->head = new_node;
		return;
	}
		
	if (pos == len) {
		new_node->prev = l->rear;
		new_node->next = NULL;
		l->rear->next = new_node;
		l->rear = new_node;
		return;
	}
	
	
	temp = l->head;
	for(i = 0; i < pos - 1; i++) {
		temp = temp->next;	
	}
	
	new_node->prev = temp;
	new_node->next = temp->next;
	temp->next->prev = new_node;
	temp->next = new_node;
}

void print_list(list l) {
	node *temp;
	temp = l.head;	
	if(temp == NULL) {
		return ;	
	}
	if(temp == l.rear){
		printw("%c", temp->data);
		return;
	}
		
	do {
		printw("%c", temp->data);
		temp = temp->next;
	}while(temp != NULL);
	
}


void save_file(list *l, char *file_name) {

	FILE *fp;
	fp = fopen(file_name, "w+");
	node* travel;
	travel = l->head;
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
	list l;
	int i, flag = 0;
	if(argc == 2) {
		init_list(&l);
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
					flag = 1;
					//x--;
					//move_left(&l);
					if(x>0)
						move(y, --x);
					refresh();
					break;
				case 'q':
					break;
				default : 
					insert(&l, ch, x);
					//addch(ch);
					//x++;
					
					clear();
						
					print_list(l);	
					move(y, ++x);
					refresh();	
					break;
			}
			refresh();
			//print_prompt();	
			if(ch == 'q') {
				save_file(&l, argv[1]);
				break;
			}
		}
		endwin();
	}
	return 0;
}
				
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
