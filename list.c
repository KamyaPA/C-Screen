#include <stdlib.h>
#include "list.h"

List list_init () {
	List new;
	new.item = NULL;
	new.next = NULL;
	new.before = NULL;
	return new;
}

List *list_add (List *list, void *item) {
	if (list->item == NULL) {
		list->item = item;
		return list;
	}
	if (list->next == NULL) {
		list->next = calloc(1, sizeof(List));
		if (list->next == NULL) return NULL;
		list->next->before = list;
	} 
	return list_add(list->next, item);
}

void list_move_step(List *list_item, int step){	
	List * tmp;
	while(step != 0){
		if(step > 0){
			if(list_item->next != NULL){
				tmp = list_item->next;
				tmp->before     = list_item->before;
				tmp->next       = list_item;
				list_item->next = tmp;
			}
			else{
				return;
			}
			step--;
		}
		else if(step < 0){
			if(list_item->before != NULL){
				tmp = list_item->before;
				tmp->next         = list_item->next;
				tmp->before       = list_item;
				list_item->before = tmp;	
			}
			else{
				return;
			}
			step++;
		}
	}
}

void list_remove (List *list) {
	if(list->item   != NULL) free (list->item);
	if(list->before != NULL) list->before->next = list->next;
	if(list->next   != NULL) list->next->before = list->before;
	free(list);
}

void list_delete (List *list){
	if(list == NULL) return;
	list_delete(list->next);
	free (list->item);
	free (list);
}
