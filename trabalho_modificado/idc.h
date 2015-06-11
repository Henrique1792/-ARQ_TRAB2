#ifndef IDC_H_
#define IDC_H_

#include "stdlib.h"
#include "stdio.h"
#include <string.h>
#include "booktag.h"

typedef struct INDICES_T{
	FILE *author;
	FILE *publisher;
}INDICES_T;


int verify_index(char *filename);
void create_index(char *source,int check);

//retorna o byte offset
int recover(FILE *idx);

//remoção no índice de um registro removido
void remotion(FILE *idx, int byteoffset);

#endif
