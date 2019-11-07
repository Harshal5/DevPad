#include<stdio.h>
#include<stdlib.h>
#include<ncurses.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include "hlist.h"

void init_hlist(hlist *hl) {
	hl->head = NULL;
	hl->rear = NULL;
}

int hlength (hlist hl) {
	hnode *temp;
	temp = hl.head;
	if(temp == NULL) {
		return 0;	
	}
	int hlen = 1;
	if(temp == hl.rear){
		return hlen;
	}
	do {
		temp = temp->next;
		hlen++;
	}while(temp != hl.rear);
	return hlen;
}

void hinsert (hlist *hl, char c, int pos) {
	hnode *temp, *new_hnode;
	int hlen, i = 0;
	hlen = hlength(*hl);
	if (pos <0 || pos > hlen)
		return;
	
	
	new_hnode = (hnode*)malloc(sizeof(hnode));
	new_hnode->data = c;
	if(hlen == 0) {
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
		
	if (pos == hlen) {
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
