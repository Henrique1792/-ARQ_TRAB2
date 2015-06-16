#include "cosec.h"

/*
   Trabalho de Organizacao de Arquivos - Trabalho 2

   Integrantes:

   Marcos Vinicius Barros L. Andrade Junqueira     numero USP: 8922393
   Rita Raad                                       numero USP: 8061452
   Henrique Fernandes de Mattos Freitas             numero USP: 8937225
   Gustavo Santiago                                numero USP: 8937416

   Descricao do arquivo idc.c: arquivo que possui a implementacao das funcoes definidas no arquivo idc.h.
   Possui funcoes relativas a manipulacao, criacao, remocao e checagem dos índices e listas.

*/


void index_match(char *author, char *publisher){
	if(author){
		FILE *data = fopen(DATAFILE_PATH,"r+");
		FILE *listaut = fopen(IDXLISTAUT_PATH,"r");
		FILE *idxaut = fopen(IDXSECAUT_PATH,"r");
		FILE *listpub = fopen(IDXLISTPUB_PATH,"r");
		FILE *idxpub = fopen(IDXSECPUB_PATH,"r");

		if(!listaut || !idxaut || !listpub || !idxpub){
			printf("\n[AVISO] Índices não existem ou estão corrompidos\n");
			return;
		}

		IDXSEC_T *readaut = read_idxsec(idxaut);
		IDXSEC_T *readpub = read_idxsec(idxpub);
		IDXLIST_T *listreadaut;
		IDXLIST_T *listreadpub;
		IDXLIST_T *listelements;
		BOOKTAG_T *booktag;

		//procura autor e publisher
		//-------------------------------------------------------------------------//
		int ok = 0, tmp = 0;
		while(readaut){
			if(strcmp(readaut->field,author) == 0){
				ok = 1;
				break;
			}
			readaut = read_idxsec(idxaut);
		}

		if(ok != 1){
			printf_colorful("\n[ERRO] Não foi possível encontrar o autor\n",ANSI_RED);
			return;
		}

		ok = 0;
		while(readpub){
			if(strcmp(readpub->field, publisher) == 0){
				ok = 1;
				break;
			}
			readpub = read_idxsec(idxpub);
		}

		if(ok != 1){
			printf_colorful("\n[ERRO] Não foi possível encontrar a editora\n",ANSI_RED);
			return;
		}
		//-------------------------------------------------------------------------//

		int i = 0; 	
		listreadaut = read_list(listaut, readaut->listPos);
		listreadpub = read_list(listpub, readpub->listPos);
		listelements = (IDXLIST_T *) malloc(sizeof(IDXLIST_T));

		while(listreadaut || listreadpub){
			if(listreadaut->offset < listreadpub->offset){
				listreadpub = read_list(listpub, listreadpub->next);
			}

			else if(listreadaut->offset > listreadpub->offset){
				listreadaut = read_list(listaut, listreadaut->next);
			}

			else if(listreadaut->offset == listreadpub->offset){
				listelements[i].offset = listreadaut->offset;
				listelements[i].next = tmp; //guarda o tamanho do registro

				listreadaut = read_list(listaut, listreadaut->next);
				listreadpub = read_list(listpub, listreadpub->next);

				if(listreadaut) realloc(listelements, sizeof(listreadaut));
				else if(listreadpub) realloc(listelements, sizeof(listreadpub));
				i++;
			}
		}

		int j = 0;
		for(j = 0; j < i; j++){
			rewind(data);
			fseek(data, listelements[j].offset, SEEK_CUR);

			booktag = get_booktag(data, &tmp);
			//print_list(listread);
			printf_booktag(booktag);
		}

		//free(listelements);

		fclose(data);
		fclose(listaut);
		fclose(listpub);
		fclose(idxaut);
		fclose(idxpub);
	}
}

void index_merge(char *author, char *publisher){
	if(author){
		FILE *data = fopen(DATAFILE_PATH,"r+");
		FILE *listaut = fopen(IDXLISTAUT_PATH,"r");
		FILE *idxaut = fopen(IDXSECAUT_PATH,"r");
		FILE *listpub = fopen(IDXLISTPUB_PATH,"r");
		FILE *idxpub = fopen(IDXSECPUB_PATH,"r");

		if(!listaut || !idxaut || !listpub || !idxpub){
			printf("\n[AVISO] Índices não existem ou estão corrompidos\n");
			return;
		}

		IDXSEC_T *readaut = read_idxsec(idxaut);
		IDXSEC_T *readpub = read_idxsec(idxpub);
		IDXLIST_T *listreadaut;
		IDXLIST_T *listreadpub;
		IDXLIST_T *listelements;
		BOOKTAG_T *booktag;

		//procura autor e publisher
		//-------------------------------------------------------------------------//
		int ok = 0, tmp = 0;
		while(readaut){
			if(strcmp(readaut->field,author) == 0){
				ok = 1;
				break;
			}
			readaut = read_idxsec(idxaut);
		}

		if(ok != 1){
			printf_colorful("\n[ERRO] Não foi possível encontrar o autor\n",ANSI_RED);
			return;
		}

		ok = 0;
		while(readpub){
			if(strcmp(readpub->field, publisher) == 0){
				ok = 1;
				break;
			}
			readpub = read_idxsec(idxpub);
		}

		if(ok != 1){
			printf_colorful("\n[ERRO] Não foi possível encontrar a editora\n",ANSI_RED);
			return;
		}
		//-------------------------------------------------------------------------//

		int i = 0; 	
		listreadaut = read_list(listaut, readaut->listPos);
		listreadpub = read_list(listpub, readpub->listPos);
		listelements = (IDXLIST_T *) malloc(sizeof(IDXLIST_T));

		while(listreadaut || listreadpub){
			if(listreadaut && listreadpub){
				if(listreadaut->offset < listreadpub->offset){
					listelements[i].offset = listreadpub->offset;
					listelements[i].next = tmp; //guarda o tamanho do registro

					listreadpub = read_list(listpub, listreadpub->next);
				}

				else if(listreadaut->offset > listreadpub->offset){
					listelements[i].offset = listreadaut->offset;
					listelements[i].next = tmp; //guarda o tamanho do registro

					listreadaut = read_list(listaut, listreadaut->next);
				}

				else if(listreadaut->offset == listreadpub->offset){
					listelements[i].offset = listreadaut->offset;
					listelements[i].next = tmp; //guarda o tamanho do registro
					//print_list(&listelements[i]); //debug

					listreadaut = read_list(listaut, listreadaut->next);
					listreadpub = read_list(listpub, listreadpub->next);
				}
			}

			else{
				if(listreadaut){
					printf("Acabou publisher\n");
					listelements[i].offset = listreadaut->offset;
					listelements[i].next = tmp; //guarda o tamanho do registro

					listreadaut = read_list(listaut, listreadaut->next);
				}

				else if(listreadpub){
					printf("Acabou author\n");
					listelements[i].offset = listreadpub->offset;
					listelements[i].next = tmp; //guarda o tamanho do registro

					listreadpub = read_list(listpub, listreadpub->next);
				}
			}

			if(listreadaut) realloc(listelements, sizeof(listreadaut));
			else if(listreadpub) realloc(listelements, sizeof(listreadpub));
			i++;
		}

		int j = 0;
		for(j = 0; j < i; j++){
			rewind(data);
			fseek(data, listelements[j].offset, SEEK_CUR);

			booktag = get_booktag(data, &tmp);
			//print_list(listread);
			printf_booktag(booktag);
		}

		//free(listelements);

		fclose(data);
		fclose(listaut);
		fclose(listpub);
		fclose(idxaut);
		fclose(idxpub);
	}
}