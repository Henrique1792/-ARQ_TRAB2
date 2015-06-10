#include "list.h"


KNOT_T *create_knot(int byte){
	KNOT_T *devolve;
	devolve=(KNOT_T *)malloc(sizeof(KNOT_T));
	if(devolve !=NULL){
		devolve->byteoffset=byte;
		devolve->nxt=NULL;
		return devolve;
	}
	return NULL;
}

void del_knot(KNOT_T **target){
	if(target!=NULL && (*target) !=NULL){
		free(*target);
		*target=NULL;
	}
}

void print_knot(KNOT_T *target){
		if(target->nxt!=NULL)
		print_knot(target->nxt);
}

LLIST_T *create_list(int valor){
	LLIST_T *devolve;
	devolve=(LLIST_T *)malloc(sizeof(LLIST_T));
	if(devolve!=NULL){
		devolve->start=NULL;
		return devolve;
	}
	return NULL;
}

int delete_list(LLIST_T **target){
	if(target==NULL || *target==NULL)
		return 0; 
	KNOT_T *paux = (*target)->start,
		   *prem = NULL;

	while(paux!=NULL){
		prem=paux;
		paux=paux->nxt;
		del_knot(&prem);
	}
	free(*target);
	*target=NULL;
	return 1;
}

int add_list(int byte, LLIST_T *target){
	KNOT_T *add=create_knot(byte);
	if(add==NULL || target==NULL)
		return 0;
	
	KNOT_T *paux=target->start;
	
	if(paux==NULL){
		target->start=add;
		return 1;
	}

	while(paux->nxt!=NULL && paux->byteoffset < byte)
		paux=paux->nxt;
	if(paux->nxt==NULL){
		paux->nxt=add;
	}
	else{
		add->nxt=paux->nxt;
		paux->nxt=add;
	}
	return 1;
}


int rm_list(int byte, LLIST_T *lista){
	if(target < 0 || lista == NULL || lista->start == NULL)
		return 0;

	KNOT_T *pant=NULL, *prem=lista->start;

	while(prem!=NULL && prem->byteoffset != byte){
		pant=prem;
		prem=prem->nxt;
	}
	if(prem==lista->start){
		lista->start=prem->nxt;
		prem->nxt=NULL;
		del_knot(&prem);
		return 1;
	}
	pant->nxt=prem->nxt;
	prem->nxt=NULL;
	del_knot(&prem);
	return 1;
}

void print_list(LLIST_T *list){
	if(list==NULL || list->start==NULL) return;

	KNOT_T *aux=list->start;
		print_knot(aux);
}
