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

void init_vlist(vlist *vl){

	vl->top = NULL;
	vl->bottom = NULL;

}


int vlength (vlist vl){

	vnode *temp;
	temp = vl.top;
	if(temp == NULL) {
		return 0;	
	}
	int vlen = 1;
	if(temp == vl.bottom){
		return vlen;
	}
	do {
		temp = temp->next;
		vlen++;
	}while(temp != vl.bottom);
	return vlen;

}
void vinsert (vlist *vl, hlist *hl, int pos){
	
	vnode *temp, *new_vnode;
	int vlen, i = 0;
	vlen = vlength(*vl);
	if (pos <0 || pos > vlen)
		return;
	
	new_vnode = (vnode*)malloc(sizeof(vnode));
	new_vnode->row = hl->head;

	if(vlen == 0) {
		vl->top = new_vnode;
		vl->bottom = new_vnode;
		new_vnode->prev = NULL;
		new_vnode->next = NULL;	
		return;
	}	
	
	if(pos == 0) {
		new_vnode->next = vl->top;
		new_vnode->prev = NULL;
		vl->top->prev = new_vnode;
		vl->top = new_vnode;
		return;
	}
		
	if (pos == vlen) {
		new_vnode->prev = vl->bottom;
		new_vnode->next = NULL;
		//new_vnode->row = NULL;
		vl->bottom->next = new_vnode;
		vl->bottom = new_vnode;
		return;
	}
	
	
	temp = vl->top;
	for(i = 0; i < pos - 1; i++) {
		temp = temp->next;	
	}
	
	new_vnode->prev = temp;
	new_vnode->next = temp->next;
	temp->next->prev = new_vnode;
	temp->next = new_vnode;

}

void vreplace(vlist *vl, hlist *hl, int y){

	vnode *vtemp;
	vtemp = vl->top;
	while(y--){
		vtemp = vtemp->next;
	}
		vtemp->row = hl->head;

}