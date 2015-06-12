#ifndef IDC_H_
#define IDC_H_

#include "stdlib.h"
#include "stdio.h"
#include <string.h>
#include "booktag.h"


typedef struct IDX_SEC{
	char *field; //author ou publisher
	int listPos; //primeira ocorrência na lista invertida
}IDXSEC_T;

typedef struct IDX_LIST{
	char *key;	//chave primária
	int offset;	//offset no arquivo de dados
	int next;	//pseudo-ponteiro para o próximo
}IDXLIST_T;

typedef struct INDICES_T{
	FILE *author;
	FILE *publisher;
	FILE *list_aut;
	FILE *list_pub;
}INDICES_T;

int get_offset(FILE *stream);

int insert_to_index(INDICES_T *idx, BOOKTAG_T *booktag, int offset);

int verify_index(char *filename);

void create_index(char *source);

#endif
