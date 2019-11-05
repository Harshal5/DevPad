#include<stdio.h>
#include<stdlib.h>
#include<ncurses.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

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
	if((l->head) == NULL) {
		l->rear = NULL;
		l->head = new_node;
	}
	else {
		new_node->prev = l->rear;
	}
	new_node->next = NULL;
	l->rear = new_node;
	
}


void print_list(list *l) {

	node* travel;
	travel = l->head;
	while(travel != NULL) {
		printf("%c", travel->data);
		travel = travel->next;
	}

}

int main(int argc, char *argv[]) {
	FILE *fp;
	char ch;
	list l;
	int i;
	//if(argc == 2) {
	//	fp = fopen(argv[1], "w+");
		initscr();
		raw();
		noecho();
		refresh();
		keypad(stdscr, TRUE);
		while(1) {
			ch = getch();
			switch(ch) {
				case 
			
			
			
			
			
			if(ch == 'q')
				break;
			addch(ch);
			//printw("%c ", ch);
		}	
		endwin();
		
	//}
	return 0;
}
