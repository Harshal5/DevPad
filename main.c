#include<stdio.h>
#include<stdlib.h>
#include<ncurses.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>


int count = 0;

typedef struct node {
	char data;
	int wn;
	struct node *prev, *next;
}node;

typedef struct list {
	node *head, *rear;
}list;

void init_list(list *l) {

	l->head = NULL;
	l->rear = NULL;

}


void append(list *l, char c) {

	node *new_node;
	new_node = (node*)malloc(sizeof(node));
	new_node->data = c;
	new_node->wn = ++count; 
	if((l->head) == NULL) {
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
	
	printw("print_list");
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

int main(int argc, char *argv[]) {

	char ch;
	list l;
	int i;
	if(argc == 2) {
		//fp = fopen(argv[1], "w+");
		init_list(&l);
		initscr();
		raw();
		noecho();
		refresh();
		keypad(stdscr, TRUE);
		while(1) {
			ch = getch();
			switch(ch) {
				case 'q':
					break;
				default : 
					append(&l, ch);
					addch(ch);	
					break;
			}
			if(ch == 'q') {
				print_list(&l);
				save_file(&l, argv[1]);
				break;
			}
		}	
		endwin();
	}
	return 0;
}
