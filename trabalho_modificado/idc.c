#include "idc.h"

/*verify_index:
 * @description
 * verifica se o índice deve ser criado.
 * @param:
 * 		char * filename: nome do arquivo de source a ser verificado.
 * @return:
 * 		0: caso não possua o mínimo necessário.
 * 		1: caso possua o mínimo necessaŕio.
 * */

int verify_index(char *filename){
	if(!filename){
		printf("não pôde abrir o arquivo");
		return 0;	
	}
	FILE *fr = fopen(filename,"r");
	int i=0,size=0;

	while(fread(&size,sizeof(int),1,fr) && i < 10){
		printf("%d\n",size);
		fseek(fr,size,SEEK_CUR);
		i++;
	}

	fclose(fr);
	if(i<10) return 0;
	else 	 return 1;
}

void create_index(char *source,int check){
	if(!check || !source) return;
	
	INDICES_T *idx = (INDICES_T *)malloc(sizeof(INDICES_T));
	int size;
	FILE *src=fopen(source,"r");
	BOOKTAG_T *reading;
	idx->author = fopen("author.ind","w+");
	idx->publisher = fopen("publisher.ind","w+");

	while(reading!=NULL){
		reading = get_booktag(src,&size);
		printf("%d \n",size);
		//fprintf(idx->author,"%ld \n",off);
	};

	//liberação das estruturas utilizadas para leitura.
	fclose(idx->author);
	fclose(idx->publisher);
	free(idx);
}
