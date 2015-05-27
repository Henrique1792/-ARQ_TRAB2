#include <stdlib.h>
#include <stdio.h>
#define DELIM '|'

char *readstr(FILE *f){
	char *str=NULL, getter='a';
	int i=0;
	if(f==NULL)return;

	do{
		getter=fgetc(f);
		str=(char *)realloc(str, (i+1)*sizeof(char));
		str[i++]=getter;	
	}while(getter!=DELIM);
	str[--i]='\0';
	return str;
}



int main(){
	FILE *fr=fopen("teste.txt","r");
	char *str;


	str=readstr(fr);
	printf("%s",str);

	free(str);
	fclose(fr);
	return 0;
}
