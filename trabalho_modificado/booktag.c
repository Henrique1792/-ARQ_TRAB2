#include <stdio.h> // usamos função de input
#include <assert.h> //utilizamos o assert para exigir parametros corretos
#include <unistd.h> // para utilizar o system("clear")
#include <stdlib.h> //utilizamos para malloc/free
#include <string.h> //utilizamos o fgets

#include "debug.h"
#include "booktag.h"
#include "screen.h"

/*
   Trabalho de Organizacao de Arquivos - Trabalho 2

   Integrantes:

   Marcos Vinicius Barros L. Andrade Junqueira     numero USP: 8922393
   Rita Raad                                       numero USP: 8061452
   Henrique Fernandes de Mattos Freitas            numero USP: 8937225
   Gustavo Santiago                                numero USP: 8937416

   Descricao do arquivo booktag.c: arquivo que possui a implementacao das funcoes definidas no arquivo booktag.h.
   Possui funcoes relativas a manipulacao, criacao, remocao e checagem dos registros(booktags) no arquivo.

   Toda memoria liberada nas funcoes sao liberadas nelas, exceto a funcao create_booktag.

 */


/**
   @brief Função create_booktag() que aloca uma estrutura do tipo BOOKTAG_T
   @return estrutura alocada
 **/
 /*atualizado*/
BOOKTAG_T *create_booktag() {

    //alocamos o novo livro
    BOOKTAG_T *book = malloc(sizeof(BOOKTAG_T)); // criamos a variavel

    //atribuindo ponteiros como NULL.
	book->title=NULL;
	book->author=NULL;
	book->publisher=NULL;
	book->language=NULL;

    return book;
}

/**
   @brief Função free_booktag() Libera uma booktag da memória
   @param BOOKTAG_T *booktag que será liberada
**/
void free_booktag(BOOKTAG_T *booktag) {
    if (booktag != NULL) { // verificamos parametro e os liberamos da memória
		free(booktag->title);
		free(booktag->author);
		free(booktag->publisher);
		free(booktag->language);
		free(booktag);
	}
}


int booktag_sizeof(BOOKTAG_T *booktag){
	if(booktag != NULL){
		int size = 0;

		size += sizeof(int); //indicador de tamanho
		size += sizeof(char) * strlen(booktag->title);
		size += sizeof(char) * strlen(booktag->author);
		size += sizeof(char) * strlen(booktag->publisher);
		size += sizeof(booktag->year);
		size += sizeof(char) * strlen(booktag->language);
		size += sizeof(booktag->pages);
		size += sizeof(booktag->price);
		size += 4 * sizeof(char); // tamanho dos delimitadores

		return size;
	}
	return -1; //não é possível operar
}


/**
   @brief Função write_booktags() recebe um booktag que irá ser gravado em um arquivo

   @param BOOKTAG_T *booktag que irá ser gravada no arquivo
   @param char filename[] nome do arquivo que será gravado
   @param int n número de booktags que serão gravadas no filename
   @return int
 **/
void write_booktags(BOOKTAG_T *booktag, char filename[]) {
	char chr = DELIM;
    //garantimos os paramêtros corretos para evitar erros no arquivo de dados
    assert(booktag != NULL || filename != NULL);

    //abrimos o arquivo de log e em caso de erro, gravamos aviso nele
    FILE *f;
    f = fopen_log(filename, "a");

    if (f == NULL) {
        printf("\n[ERRO] erro na abertura do arquivo\n");
        return;
    }

    int tam = booktag_sizeof(booktag);
    // gravamos os campos no arquivo
    fwrite_log(&tam, sizeof(int), 1, f);
	
    fwrite_log(booktag->title, sizeof(char), strlen(booktag->title), f);
	fwrite_log(&chr,sizeof(char),1,f); // separador

	fwrite_log(booktag->author, sizeof(char),strlen(booktag->author), f);
	fwrite_log(&chr,sizeof(char),1,f); // separador

	fwrite_log(booktag->publisher, sizeof(char),strlen(booktag->publisher), f);
    fwrite_log(&chr,sizeof(char),1,f); // separador 

	fwrite_log(&booktag->year, sizeof(int), 1, f);

	fwrite_log(booktag->language, sizeof(char)*strlen(booktag->language), 1, f);
	fwrite_log(&chr,sizeof(char),1,f); // separador 

	fwrite_log(&booktag->pages, sizeof(int), 1, f);
	fwrite_log(&booktag->price, sizeof(float), 1, f);

	printf_debug("\n\nGravação com sucesso\n\n");
    fclose_log(f); //fechamos o log
}


/**
   Funcao interna str_untrim() coloca uma \0 no final da string

   @paramp char string[] string que terá o \0 no final
 **/
static void str_untrim(char string[]) {

    //verificamos parametro e inserirmos \0 no final da string
    if (string != NULL) {
        int i = strlen(string);
        string[i+1] = '\0';
    }


}
/**
 * Função que le uma booktag no arquivo de dados
 * @param[in] FILE ponteiro para o arquivo
 * @param[in,out] *tam tamanho do booktag lido
 * @return BOOKTAG_T lida
 **/
BOOKTAG_T *get_booktag(FILE *f, int *tam){
	int size;
	if(f==NULL){
		printf("AVISO: erro no ponteiro do arquivo");
		return NULL;
	}

	if(feof(f)) return NULL;

	BOOKTAG_T *getter = create_booktag();

	if(!fread(&size,sizeof(int),1,f)) return NULL;
	*tam = size;
	
	// lendo (utilizamos a funçaõ readstr para ler até o delimitador
	getter->title = readstr(f);	
	getter->author = readstr(f);
	getter->publisher = readstr(f);
	fread(&getter->year,sizeof(int),1,f);
	getter->language = readstr(f);
	fread(&getter->pages,sizeof(int),1,f);
	fread(&getter->price,sizeof(float),1,f);

	return getter;
}


/**
   Funcao read_booktag() lê do arquivo de nome filename e retorna o primeiro booktag lido
   função para casos em que precisamos checar se há pelo menos uma booktag no arquivo.

   @param char filename[] nome do arquivo a ser lido
   @return BOOKTAG_T  lida
 **/
BOOKTAG_T *read_booktag(FILE *f) {
	int size;
    if(f==NULL){
        printf("AVISO: erro no ponteiro do arquivo (==NULL)" );
        return NULL;
    }

    //alocamos a estrutura temporária e o ponteiro para o arquivo
    BOOKTAG_T *booktag_temp = create_booktag();


    //lemos uma booktag do arquivo
    fread(&size,sizeof(int),1,f);

    booktag_temp->title=readstr(f);

    booktag_temp->author=readstr(f);

    booktag_temp->publisher=readstr(f);

    fread(&booktag_temp->year,sizeof(int), 1,f);

    booktag_temp->language=readstr(f);

    fread(&booktag_temp->pages,sizeof(int),1,f);

    fread(&booktag_temp->price,sizeof(float),1,f);


    //fechamos e retornamos
    fclose_log(f);
    return booktag_temp;
}


/**
 * Le uma string ate encontrar o delimitador
 *@param: target é o arquivo a ser lido.
 *@return: string lida.
 **/
static char *readstr(FILE *target){
	char getter='a';
	char *string = NULL;
	int i=0;
	do{
		getter=fgetc(target);
        // WARNING here
		string = (char *)realloc(string,(i+1)*sizeof(char));
        if (!string) {
            free(string);
        }
		string[i++]=getter;
        if (feof(target)) break;
	} while(getter!= DELIM);
	string[--i]='\0';
	return string;
}


/**
   Função printf_booktag() que imprime uma booktag
   @param booktag a ser impressa
 **/
void printf_booktag(BOOKTAG_T *booktag) {
    //verificamos se o paramẽtro está ok
    if (booktag == NULL) {
        printf_error("[ERRO] booktag nula, precisa de paramêtro para impressão");
        return;
    }
    
    // checagens para ver se o conteúdo está valido
    if (sizeof(booktag) < 5) return;
    //vemos se o tamanho é zero, se for saimos porque naõ é um registro válido
    if (strlen(booktag->title) == 0 || booktag->title == NULL) return;
    if (booktag->price == 0 || booktag->pages == 0 || booktag->price == 0) return;
	
 	// vemos se a booktag está marcada para remoção
    if(booktag->title[0] == '*'){
        printf_separator();
        printf_debug("Registro removido");
        printf_separator();
        return;
    }
	
	// imprimimos o conteudo    
    printf_separator();

    printf_colorful("Titulo: ", ANSI_CYAN);
    printf_colorful(booktag->title, ANSI_MAGENTA);

    printf_colorful("\nAutor: ", ANSI_CYAN);
    printf_colorful(booktag->author, ANSI_MAGENTA);


    printf_colorful("\nEditora: ", ANSI_CYAN);
    printf_colorful(booktag->publisher, ANSI_MAGENTA);

    printf_colorful("\nIdioma: ", ANSI_CYAN);
    printf_colorful(booktag->language, ANSI_MAGENTA);

    printf_colorful("\nAno: ", ANSI_CYAN);
    char year[100];
    sprintf(year, "%d", booktag->year);
    printf_colorful(year, ANSI_MAGENTA);

    printf_colorful("\nPaginas: ", ANSI_CYAN);
    char pages[100];
    sprintf(pages, "%d", booktag->pages);
    printf_colorful(pages, ANSI_MAGENTA);


    printf_colorful("\nPreco: ", ANSI_CYAN);
    char price[100];
    sprintf(price, "R$%0.2f", booktag->price);
    printf_colorful(price, ANSI_MAGENTA);


    printf_separator();
}


/**
   Funcao read_booktag_list() que le uma lista de booktags de um arquivo e os imprimi
   @param char filename[] nome do arquivo a ser lido
 **/
void read_booktag_list(char filename[]) {

    //verificamos os parametros e em caso de erro, avisamos
    if (filename == NULL) {
        printf("\n[AVISO] filename precisa ser o nome do arquivo a ser utilizado\n");
        return;
    }

    //alocamos a estrutura temporária e o ponteiro para o arquivo
    BOOKTAG_T *booktag_temp = create_booktag();
    FILE *f = fopen(filename, "rb");

    //verificamos o ponteiro de arquivo, em caso de erro, avisamos
    if(f == NULL) {
        printf_error("\n[ERRO]Erro na abertura do arquivo\n");
        free(booktag_temp);
        return;
    }

    int i = 0, size = 0;
    //lemos uma estrutura do arquivo e mostramos seu RRN e contéudo
    while(booktag_temp) {

        booktag_temp = get_booktag(f, &size);

        if(booktag_temp){
            printf("\n[%d] tamanho: %d", i, size);
            printf_booktag(booktag_temp);
            free_booktag(booktag_temp);
        }

        i++;

    }

    //liberamos memória e fechamos o ponteiro
    fclose(f);
    free_booktag(booktag_temp);

    printf_debug("Digite 1 para sair do relatorio: ");

    int op;
    scanf("%d", &op);
    return;
}


/**
   Funcao read_booktag_one() que le uma lista de booktags de um arquivo e os imprimi
   @param char filename[] nome do arquivo a ser lido
 **/
void read_booktag_list_one(char filename[]) {

    //verificamos os parametros e em caso de erro, avisamos
    if (filename == NULL) {
        printf("\n[AVISO] filename precisa ser o nome do arquivo a ser utilizado\n");
        return;
    }

    //alocamos a estrutura temporária e o ponteiro para o arquivo
    BOOKTAG_T *booktag_temp = create_booktag();
    FILE *f = fopen(filename, "rb");

    //verificamos o ponteiro de arquivo, em caso de erro, avisamos
    if(f == NULL) {
        printf_error("\n[ERRO]Erro na abertura do arquivo\n");
        free(booktag_temp);
        return;
    }
    int i = 0;
    int cont = 1;
    int size = 0;
    while(!feof(f) && cont == 1) {

        booktag_temp = get_booktag(f, &size);

        if(booktag_temp){
            printf("\n[%d] tamanho: %d", i, size);
            printf_booktag(booktag_temp);
            free_booktag(booktag_temp);
            i++;

            printf_colorful("\n\nDigite 1 para continuar a impressao ", ANSI_CYAN);
            rewind(stdin);
            scanf("%d", &cont);
        }

    }



    printf_separator();
    printf_debug("\n\nFinal do arquivo");
    printf_separator();


    fclose(f);
    free_booktag(booktag_temp);
    return;
}




/**
   Função markrem_booktag() remove logicamente uma booktag
   A função recebe o arquivo de dados aberto e o offset do registro a ser removido

   @param FILE *data arquivo de dados
   @param int offset a ser removido
   @return int se removido ou não (-1 para erro, 1 para encontrado)
**/
int markrem_booktag(FILE *data, int offset){

    //checamos os parametros
    if (!data) {
        printf("\n[ERRO] Erro no arquivo\n");
        return -1;
    }

    char *mark = (char *) calloc(1,sizeof(char));
    mark[0] = CHAR_REM;

    rewind(data);
   	// marcamos o espaço logo após do tamanho
    fseek(data, offset + sizeof(int), SEEK_CUR);

    fwrite(mark,sizeof(char),1,data);
    return 1;
}


/**
* recover_year 
* recupera registro de acordo com ano
* @param FILE* file do arquivo a ser procurado
* @param int year ano á ser pesquisado
*/
void recover_year (char filename[], int year){
    //verificamos os parametros e em caso de erro, avisamos
    if (filename == NULL) {
        printf("\n[AVISO] filename precisa ser o nome do arquivo a ser utilizado\n");
        return;
    }

    //alocamos a estrutura temporária e o ponteiro para o arquivo
    BOOKTAG_T *booktag_temp = create_booktag();
    FILE *f = fopen(filename, "r");

    //verificamos o ponteiro de arquivo, em caso de erro, avisamos
    if(f == NULL) {
        printf_error("\n[ERRO]Erro na abertura do arquivo\n");
        free(booktag_temp);
        return;
    }

    int ok = 0, size = 0;
    //lemos uma estrutura do arquivo e mostramos seu RRN e contéudo
    while(booktag_temp) {

        booktag_temp = get_booktag(f, &size);

        if(booktag_temp){
            if(booktag_temp->year == year){
                printf_booktag(booktag_temp);
                ok = 1;
            }
            free_booktag(booktag_temp);
        }

    }

    //liberamos memória e fechamos o ponteiro
    fclose(f);
    free_booktag(booktag_temp);

    if(!ok) printf_colorful("\nNenhum registro encontrado\n",ANSI_CYAN);
    printf_debug("\nDigite 1 para sair do relatorio: ");

    int op;
    scanf("%d", &op);
    return;
}
