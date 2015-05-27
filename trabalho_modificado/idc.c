#include <stdio.h>
#include <stdlib.h>
//arquivos nossos
#include "idc.h"
#include "booktag.h"
#include "debug.h"

//funçoes de manipulação do índice primário
FILE *create_idxprim(char *name){
	if(name){
		FILE *idxprim;

		idxprim = fopen_log(IDXPRIM_PATH,"w+");
		
		if(idxprim){
			printf("[MESSAGE]Primary index created :D\n");
			return idxprim;
		}
	}

	printf("[ERROR]Primary index NOT created D:\n");
	return NULL;

}

int add_idxprim(BOOKTAG_T *book){
	if(book){
		return 1;
	}
	return 0;
}

int rm_idxprim(FILE *target, const void *field){
	if(target && field){
		return 1;
	}
	return 0;
}

BOOKTAG_T *search_idxprim(FILE *target, int rrn){
	if(target && rrn >= 0){
		BOOKTAG_T *tmptag = (BOOKTAG_T *)malloc(sizeof(BOOKTAG_T));
		return tmptag;
	}
	return NULL;
}

//funções de manipulação do índice secundário
/*create: cria um IDXSEC_T VAZIO:
 * 		  arquivo aberto com nome "name"
 * 		  ponteiro duplicate é NULL
*/
IDXSEC_T *create_idxsec(char *name){
	if(name){
		IDXSEC_T *idxsec = (IDXSEC_T *)malloc(sizeof(IDXSEC_T));

		if(idxsec){
			idxsec->idxsec = fopen_log(IDXSEC_PATH,"w+");

			if(idxsec->idxsec){
				printf("[MESSAGE]Primary index created :D\n");
				return idxsec;
			}
		}
	}

	printf("[ERROR]Primary index NOT created D:\n");
	return NULL;
}

/*add: construirá o índice secundário
 *     a partir de idxprim, adicionando 
 *     as referências em target
 *     
 *     @return:
 *     0: caso não possa gerar o conteúdo em target
 *     1: caso a inserção seja feita.
*/
int add_idxsec(FILE *idxprim, IDXSEC_T *target){
	if(idxprim && target){
		return 1;
	}
	return 0;
}
	
/*rm: remoção das referência
 *
*/
int rm_idxsec(IDXSEC_T *target, char *field){
	if(target && field){
		return 1;
	}
	return 0;
}

void recover_idxsec_byfield(IDXSEC_T *target, char *field){
	if(target && field){

	}
}

BOOKTAG_T *search_idxsec(IDXSEC_T *target,FILE *idxprim, const void *field){
	if(target && idxprim){
		BOOKTAG_T *tmptag = (BOOKTAG_T *)malloc(sizeof(BOOKTAG_T));
		return tmptag;
	}
	return NULL;
}