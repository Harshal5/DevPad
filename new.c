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

node *cursor;

void append(list *l, char c) {

	node *new_node;
	new_node = (node*)malloc(sizeof(node));
	new_node->data = c;

	if(cursor != NULL) {
		if((cursor->next) != NULL) {
		
			(new_node->next) = (cursor->next);	
			(new_node->prev) = (cursor);	
			(cursor->next) = (new_node);
			cursor = cursor->next;
			return;	
		 		
		}
	}
	
	if((l->head) == NULL) {			//initially
		new_node->prev = NULL;
		l->head = new_node;
	}	
	else {
		new_node->prev = l->rear;
		l->rear->next = new_node;
	}
	new_node->next = NULL;
	l->rear = new_node;
	
}


void print_list(list *l) {
	
	node* travel;
	int count= 0;
	travel = l->head;
	while(travel != NULL) {
		printw("%c", travel->data);
		travel = travel->next;
	}
	
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


void move_left(list *l) {
	
	cursor = cursor->prev;
}


void init_cursor(list *l) {
	
	cursor = l->rear;	
	
}

int main(int argc, char *argv[]) {

	char ch;
	list l;
	int i, flag = 0;
//	printf("ch");
	if(argc == 2) {
		//fp = fopen(argv[1], "w+");
		init_list(&l);
		initscr();
		raw();
		noecho();
		refresh();
		keypad(stdscr, TRUE);
		move(0, 0);
		getyx(stdscr, y, x);
		
		init_cursor(&l);
	
		while(1) {
			ch = getch();
			
//	printw("ch");
			switch(ch) {
				case (char)KEY_LEFT:
//					printw("left");
					flag = 1;
					move_left(&l);
					move(y, --x);
					refresh();
					break;
				case 'q':
					break;
				case '\n':
				
					append(&l, ch);
					addch(ch);
					move(++y, x=0);
					if(flag == 0)
						init_cursor(&l);	
					refresh;
					break;	
				default : 
					append(&l, ch);
					//addch(ch);
					move(y, ++x);
					print_list(&l);
					if(flag == 0)
						init_cursor(&l);	
					break;
			}
			refresh();	
			if(ch == 'q') {
				//print_list(&l);
				save_file(&l, argv[1]);
				break;
			}
		}	
		endwin();
	}
	return 0;
}
