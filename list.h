#ifndef LIST_H_
#define LIST_H_

#include <stdlib.h>
#include <stdio.h>


typedef struct KNOT_T{
	int byteoffset
	struct KNOT_T *nxt;
}KNOT_T;

typedef struct LLIST_T{
	KNOT_T *start;
}LLIST_T;


KNOT_T *create_knot(int byte)

void del_knot(KNOT_T **target);

void print_knot(KNOT_T *target);

LLIST_T *create_list(int valor)

int delete_list(LLIST_T **target);

int add_list(int byte, LLIST_T *target);


int rm_list(int byte, LLIST_T *lista);

void print_list(LLIST_T *list);


#endif