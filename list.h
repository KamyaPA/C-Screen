typedef struct List {
	void *item;
	struct List *next;
	struct List *before;
} List;

List list_init ();
void list_remove (List *list_item);
List *list_add (List *list, void *item);
void list_delete (List *list);
void list_move_step(List *list_item, int step);

