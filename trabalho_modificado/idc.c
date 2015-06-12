#include "idc.h"

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
int list_find_pos(char filename[], char *key){
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
		list_aut->key = booktag->title;
		list_aut->offset = offset;
		list_aut->next = pos;
		fprintf(idx->list_aut, "%s|%d|%d\n", list_aut->key, list_aut->offset, list_aut->next);
		fclose(idx->list_aut);
	}

	//escreve no índice de author
	if(idx->author){
		author->field = booktag->author;
		author->listPos = list_find_pos(IDXLISTAUT_PATH,list_aut->key);
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
		list_pub->key = booktag->title;
		list_pub->offset = offset;
		list_pub->next = pos;
		fprintf(idx->list_pub, "%s|%d|%d\n", list_pub->key, list_pub->offset, list_pub->next);
		fclose(idx->list_pub);
	}

	//escreve no índice de publisher
	if(idx->publisher){
		publisher->field = booktag->publisher;
		publisher->listPos = list_find_pos(IDXLISTPUB_PATH,list_pub->key);
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
