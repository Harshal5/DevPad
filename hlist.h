typedef struct hnode {
	char data;
	struct hnode *prev, *next;
}hnode;

typedef struct hlist {
	hnode *head, *rear;
}hlist;

void init_hlist(hlist *hl);
int hlength (hlist hl);
void hinsert (hlist *hl, char c, int pos);
void print_hlist(hlist hl);
void hbreak(hlist *hl, int x);