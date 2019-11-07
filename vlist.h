typedef struct vnode {
	hnode *row;
	struct vnode *prev, *next;
}vnode;

typedef struct vlist {
	vnode *top, *bottom;
}vlist;

void init_vlist(vlist *vl);
int vlength (vlist vl);
void vinsert (vlist *vl, hlist *hl, int pos);
void vreplace(vlist *vl, hlist *hl, int y);