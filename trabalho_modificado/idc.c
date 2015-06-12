#include "idc.h"

#include "list.h"

/**
   Função get_offset() apenas deixa ftell() mais didático, para
   facilitar seu uso dentro do código

   @return ftell()
 **/
int get_offset(FILE *stream){
	return ftell(stream);
}

//função interna de readline
char *readline(FILE *stream){
	char getter='a';
	char *string = NULL;
	int i=0;

	do{
		getter = fgetc(stream);
		if(getter == EOF) return NULL;
        // WARNING here
		string = (char *)realloc(string,(i+1)*sizeof(char));
        if (!string) {
            free(string);
        }
		string[i++]=getter;
        if (feof(stream)) break;
	} while(getter != '\n');

	return string;
}

//função interna de encontrar a posição na lista
int list_find_pos(char filename[]){
	FILE *stream = fopen(filename,"r");

	rewind(stream);

	int i=-1;
	char *line;

	line = readline(stream);
	while(line){
		free(line);
		line = readline(stream);
		i++;
	}

	fclose(stream);

	return i;
}

void free_list(IDXLIST_T *list){
	free(list);
}

void free_idxsec(IDXSEC_T *idx){
	free(idx->field);
	free(idx);
}


void print_list(IDXLIST_T *idx){
	if(idx  ){
		printf_colorful("\t[Offset]: ",ANSI_CYAN);
		printf("%d\n",idx->offset);
		printf_colorful("\t[Próximo]: ",ANSI_CYAN);
		printf("%d\n\n",idx->next);
	}
}

void print_idxsec(IDXSEC_T *idx){
	if(idx && idx->field){
		printf_colorful("\t[Campo]: ",ANSI_CYAN);
		printf("%s\n",idx->field);
		printf_colorful("\t[Posição]: ",ANSI_CYAN);
		printf("%d\n\n",idx->listPos);
	}
}

IDXLIST_T *read_list(FILE *file, int pos){
	if(file && pos != -1){
		//modo -2 de pos lê sequencialmente
		if(pos!=-2) rewind(file);

		int i=0;
		char *line = readline(file);
		if(!line) return NULL;

		IDXLIST_T *read = (IDXLIST_T *) malloc(sizeof(IDXLIST_T));
		if(pos!=-2){
			while(line && i < pos){
				free(line);
				line = readline(file);
				i++;
			}
		}

		read->offset = atoi(strtok(line,"|"));
		read->next = atoi(strtok(NULL,"|"));

		return read;
	}
	return NULL;
}

IDXSEC_T *read_idxsec(FILE *file){
	if(file){
		char *tmp = readline(file);
		if(!tmp) return NULL;

		IDXSEC_T *read = (IDXSEC_T *) malloc(sizeof(IDXSEC_T));

		read->field = strtok(tmp,"|");
		read->listPos = atoi(strtok(NULL,"|"));
		return read;
	}
	return NULL;
}

void index_show_lists(){
	FILE *listaut = fopen(IDXLISTAUT_PATH,"r");
	FILE *listpub = fopen(IDXLISTPUB_PATH,"r");
	if(!listaut || !listpub){
		printf("\n[AVISO] Listas não existem ou estão corrompidas\n");
		return;
	}

	IDXLIST_T *listread;

	printf_colorful("Lista gerada para Author:\n",ANSI_CYAN);
	listread = read_list(listaut, -2);
	while(listread){
		print_list(listread);
		free_list(listread);
		listread = read_list(listaut, -2);
	}

	printf_colorful("\nLista gerada para Publisher:\n",ANSI_CYAN);
	listread = read_list(listpub, -2);
	while(listread){
		print_list(listread);
		free_list(listread);
		listread = read_list(listpub, -2);
	}

	fclose(listaut);
	fclose(listpub);
}

void index_show_index(){
	FILE *idxaut = fopen(IDXSECAUT_PATH,"r");
	FILE *idxpub = fopen(IDXSECPUB_PATH,"r");
	if(!idxaut || !idxpub){
		printf("\n[AVISO] Índices não existem ou estão corrompidos\n");
		return;
	}

	IDXSEC_T *idxread;

	printf_colorful("Índice gerado para Author:\n",ANSI_CYAN);
	idxread = read_idxsec(idxaut);
	while(idxread){
		print_idxsec(idxread);
		free_idxsec(idxread);
		idxread = read_idxsec(idxaut);
	}

	printf_colorful("\nÍndice gerado para Publisher:\n",ANSI_CYAN);
	idxread = read_idxsec(idxpub);
	while(idxread){
		print_idxsec(idxread);
		free_idxsec(idxread);
		idxread = read_idxsec(idxpub);
	}

	fclose(idxaut);
	fclose(idxpub);
}

//funçao modificada para adicionar byteoffset encontrado na lista bytelist
LLIST_T *index_search_publisher(char *publisher, LLIST_T *bytelist){
	if(publisher){
		FILE *data = fopen(DATAFILE_PATH,"r");
		FILE *list = fopen(IDXLISTPUB_PATH,"r");
		FILE *idx = fopen(IDXSECPUB_PATH,"r");

		if(!list || !idx){
			printf("\n[AVISO] Índices não existem ou estão corrompidos\n");
			return;
		}

		IDXSEC_T *read = read_idxsec(idx);
		IDXLIST_T *listread;
		BOOKTAG_T *booktag;

		int ok = 0, tmp = 0;
		while(read){
			//print_idxsec(read);
			if(strcmp(read->field,publisher) == 0){
				ok = 1;
				break;
			}
			read = read_idxsec(idx);
		}

		if(ok != 1){
			printf("\nNão foi possível encontrar a editora\n");
			return;
		}

		listread = read_list(list, read->listPos);
		while(listread){
			rewind(data);
			fseek(data, listread->offset, SEEK_CUR);
			booktag = get_booktag(data, &tmp);
			//print_list(listread);
			//printf_booktag(booktag);
			add_list(listread->offset,bytelist);
			listread = read_list(list, listread->next);
		}

	}
	return bytelist;
}

//funçao modificada para adicionar byteoffset encontrado na lista bytelist
LLIST_T *index_search_author(char *author, LLIST_T *bytelist){
	if(author){
		FILE *data = fopen(DATAFILE_PATH,"r");
		FILE *list = fopen(IDXLISTAUT_PATH,"r");
		FILE *idx = fopen(IDXSECAUT_PATH,"r");

		if(!list || !idx){
			printf("\n[AVISO] Índices não existem ou estão corrompidos\n");
			return;
		}

		IDXSEC_T *read = read_idxsec(idx);
		IDXLIST_T *listread;
		BOOKTAG_T *booktag;

		int ok = 0, tmp = 0;
		while(read){
			//print_idxsec(read);
			if(strcmp(read->field,author) == 0){
				ok = 1;
				break;
			}
			read = read_idxsec(idx);
		}

		if(ok != 1){
			printf("\nNão foi possível encontrar o autor\n");
			return;
		}

		listread = read_list(list, read->listPos);
		while(listread){
			rewind(data);
			fseek(data, listread->offset, SEEK_CUR);
			booktag = get_booktag(data, &tmp);
			//print_list(listread);
			//printf_booktag(booktag);
			add_list(listread->offset,bytelist);
			listread = read_list(list, listread->next);
		}

	}
	return bytelist;
}

/**
   Função insert_to_index() insere em todos os índices e lista, tra-
   tando as colisões e atualizando todas as ocorrências
   @return -1 se ocorreu algum erro, 1 se foi possível inserir
 **/
int insert_to_index(INDICES_T *idx, BOOKTAG_T *booktag, int offset){
	if(!idx){
		printf("[AVISO] Erro ao recuperar índices\n");
		return -1;
	}

	if(!booktag){
		printf("[AVISO] Erro ao recuperar booktag\n");
		return -1;
	}

	idx->author = fopen(IDXSECAUT_PATH,"r");
	idx->publisher = fopen(IDXSECPUB_PATH,"r");
	idx->list_aut = fopen(IDXLISTAUT_PATH,"a");
	idx->list_pub = fopen(IDXLISTPUB_PATH,"a");

	IDXLIST_T *list_aut = (IDXLIST_T *) malloc(sizeof(IDXLIST_T));
	IDXLIST_T *list_pub = (IDXLIST_T *) malloc(sizeof(IDXLIST_T));
	IDXSEC_T *author = (IDXSEC_T *) malloc(sizeof(IDXSEC_T));
	IDXSEC_T *publisher = (IDXSEC_T *) malloc(sizeof(IDXSEC_T));

	//operações da parte de author
	//############################################################################//

	//carrega o índice secundário e verifica se já houve ocorrência de author
	//-----------------------------------------------------------------------//
	rewind(idx->author);

	char *tmp;
	char *line = readline(idx->author);
	int pos = -1, tmpseek = 0, seekpos = 0;

	//aqui percorre as linhas para tratar colisão
	while(line){
		tmp = strtok(line,"|");

		if(strcmp(tmp, booktag->author) == 0){	//lê a linha e verifica se
			pos = atoi(strtok(NULL,"|"));		//há colisão de author
			seekpos = tmpseek; //guarda a posição para seek
			break;
		}

		tmpseek = get_offset(idx->author); //corredor de offsets do índice
		line = readline(idx->author);
	}

	fclose(idx->author);
	idx->author = fopen(IDXSECAUT_PATH,"r+");
	//-----------------------------------------------------------------------//

	//escreve na lista invertida
	if(idx->list_aut){
		list_aut->offset = offset;
		list_aut->next = pos;
		fprintf(idx->list_aut, "%d|%d\n", list_aut->offset, list_aut->next);
		fclose(idx->list_aut);
	}

	//escreve no índice de author
	if(idx->author){
		author->field = booktag->author;
		author->listPos = list_find_pos(IDXLISTAUT_PATH);
		if(pos != -1) 	fseek(idx->author,seekpos,SEEK_CUR);
		else			fseek(idx->author,0,SEEK_END);
		fprintf(idx->author, "%s|%d\n", author->field, author->listPos);
		fclose(idx->author);
	}

	//############################################################################//

	//operações da parte de publisher
	//############################################################################//

	//carrega o índice secundário e verifica se já houve ocorrência de author
	//-----------------------------------------------------------------------//
	rewind(idx->publisher);

	line = readline(idx->publisher);
	pos = -1;
	tmpseek = 0;
	seekpos = 0;

	//aqui percorre as linhas para tratar colisão
	while(line){
		tmp = strtok(line,"|");

		if(strcmp(tmp, booktag->publisher) == 0){	//lê a linha e verifica se
			pos = atoi(strtok(NULL,"|"));			//há colisão de author
			seekpos = tmpseek; //guarda a posição para seek
			break;
		}

		tmpseek = get_offset(idx->publisher); //corredor de offsets do índice
		line = readline(idx->publisher);
	}

	fclose(idx->publisher);
	idx->publisher = fopen(IDXSECPUB_PATH,"r+");
	//-----------------------------------------------------------------------//

	//escreve na lista invertida
	if(idx->list_pub){
		list_pub->offset = offset;
		list_pub->next = pos;
		fprintf(idx->list_pub, "%d|%d\n",  list_pub->offset, list_pub->next);
		fclose(idx->list_pub);
	}

	//escreve no índice de publisher
	if(idx->publisher){
		publisher->field = booktag->publisher;
		publisher->listPos = list_find_pos(IDXLISTPUB_PATH);
		if(pos != -1) 	fseek(idx->publisher,seekpos,SEEK_CUR);
		else			fseek(idx->publisher,0,SEEK_END);
		fprintf(idx->publisher, "%s|%d\n", publisher->field, publisher->listPos);
		fclose(idx->publisher);
	}
	//############################################################################//

	return 1;
}

/**
   Função verify_index() verifica se os índices devem ser criados
   @return 0 se não é para criar, 1 se é para criar
 **/
int verify_index(char *filename){
	if(!filename){
		printf("Não pôde abrir o arquivo");
		return 0;
	}

	FILE *fr = fopen(filename,"r");
	if(!fr){
		return 0;
	}

	int i=0,size=0;

	while(fread(&size,sizeof(int),1,fr) && i < 10){
		//printf("%d\n",size);
		size -= sizeof(int);
		fseek(fr,size,SEEK_CUR);
		i++;
	}

	fclose(fr);
	if(i != 10) return 0;
	else		return 1;
}

/**
   Função create_index() cria todos os índices
 **/
void create_index(char *source){
	if(!source) return;

	FILE *src = fopen(source,"r");
	if(!src){
		return;
	}

	INDICES_T *idx = (INDICES_T *) malloc(sizeof(INDICES_T));
	BOOKTAG_T *reading;

	idx->author = fopen(IDXSECAUT_PATH,"w+");
	idx->publisher = fopen(IDXSECPUB_PATH,"w+");
	idx->list_aut = fopen(IDXLISTAUT_PATH,"w+");
	idx->list_pub = fopen(IDXLISTPUB_PATH,"w+");
	fclose(idx->author);
	fclose(idx->publisher);
	fclose(idx->list_aut);
	fclose(idx->list_pub);

	int offset = 0, size = 0;

	reading = get_booktag(src, &size);
	insert_to_index(idx, reading, offset);

	while(reading){
		offset = get_offset(src);
		reading = get_booktag(src, &size);

		if(reading){
			insert_to_index(idx, reading, offset);
		}
		//fprintf(idx->author,"%ld \n",off);
	};

	//liberação das estruturas utilizadas para leitura.
	free(idx);
}

