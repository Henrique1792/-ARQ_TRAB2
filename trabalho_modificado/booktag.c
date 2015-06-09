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


static char *readstr(FILE *target);

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

    //abrimos arquivo de log, caso de erro, mandamos msg
    FILE *fp = fopen_log(LOG_FILENAME, "a");


    if (fp != NULL) { // em caso de sucesso avisamos no arquivo
        fprintf(fp, "********************************** Registro novo ******************************\n\n");
    } else
        fprintf(fp, "\n\n[AVISO] erro na abertura/escrita do arquivo de log\n\n");

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

/**
   @brief Função write_booktags() recebe um booktag que irá ser gravado em um arquivo

   @param BOOKTAG_T *booktag que irá ser gravada no arquivo
   @param char filename[] nome do arquivo que será gravado
   @param int n número de booktags que serão gravadas no filename
   @return int
 **/

 /**alterado.*/
void write_booktags(BOOKTAG_T *booktag, char filename[]) {
	char chr='|';
    //garantimos os paramêtros corretos para evitar erros no arquivo de dados
    assert(booktag != NULL || filename != NULL);

    //abrimos o arquivo de log e em caso de erro, gravamos aviso nele
    FILE *f;
    f = fopen_log(filename, "a");

    if (f == NULL) {
        printf("\n[ERRO] erro na abertura do arquivo\n");
        return;
    }

    int tam = sizeof(booktag);
    // gravamos os campos no arquivo
    fwrite_log(&tam, sizeof(int), 1, f);

    fwrite_log(booktag->title, sizeof(char),strlen(booktag->title), f);
	fwrite_log(&chr,sizeof(char),1,f);

	fwrite_log(booktag->author, sizeof(char),strlen(booktag->author), f);
	fwrite_log(&chr,sizeof(char),1,f);

	fwrite_log(booktag->publisher, sizeof(char),strlen(booktag->publisher), f);
    fwrite_log(&chr,sizeof(char),1,f);

	fwrite_log(&booktag->year, sizeof(int), 1, f);

	fwrite_log(booktag->language, sizeof(char)*strlen(booktag->language), 1, f);
	fwrite_log(&chr,sizeof(char),1,f);

	fwrite_log(&booktag->pages, sizeof(int), 1, f);
	fwrite_log(&booktag->price, sizeof(float), 1, f);

	printf_debug("\n\nGravação com sucesso\n\n");
    fclose_log(f); //fechamos o log
}


/**
   Funcao interna str_untrim() coloca uma \0 no final da string

   @paramp char string[] string que terá o \0 no final
 **/
void str_untrim(char string[]) {

    //verificamos parametro e inserirmos \0 no final da string
    if (string != NULL) {
        int i = strlen(string);
        string[i+1] = '\0';
    }


}

/**
   Funcao read_booktag() lê do arquivo de nome filename e retorna o primeiro booktag

   @param char filename[] nome do arquivo a ser lido
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
    if (sizeof(booktag) < 5) return;
    //vemos se o tamanho é zero, se for saimos porque naõ é um registro válido
    if (strlen(booktag->title) == 0 || booktag->title == NULL) return;
    if (booktag->price == 0 || booktag->pages == 0 || booktag->price == 0) return;

    if(booktag->title[0] == '*'){
        printf_separator();
        printf_debug("Registro removido");
        printf_separator();
        printf_separator();
        return;
    }
    printf_separator();

    printf_colorful("\nTitulo: ", ANSI_CYAN);
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




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// funcoes nao alteradas ///////////////////////////////////////////// ////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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

    int size = 0;
    //lemos uma estrutura do arquivo e mostramos seu RRN e contéudo
    while(!feof(f)) {
//    while(fread(&size,sizeof(int),1,f) != 0) {
        fread(&size, sizeof(int), 1, f);

        booktag_temp->title=readstr(f);

        booktag_temp->author=readstr(f);

        booktag_temp->publisher=readstr(f);

        fread(&booktag_temp->year,sizeof(int), 1,f);

        booktag_temp->language=readstr(f);

        fread(&booktag_temp->pages,sizeof(int),1,f);

        fread(&booktag_temp->price,sizeof(float),1,f);

        printf_booktag(booktag_temp);

        printf_separator();
    }

    //liberamos memória e fechamos o ponteiro
    printf_debug("Digite 1 para sair do relatorio: ");
    int op;
    scanf("%d", &op);
    if (op == 1) {
        fclose(f);
        free_booktag(booktag_temp);
        return;

    } else {
        sleep(10);
        fclose(f);
        free_booktag(booktag_temp);
        return;
    }
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
    int cont = 1;
    int size = 0;
    while(!feof(f) && cont == 1) {

        fread(&size, sizeof(int), 1, f);

        booktag_temp->title=readstr(f);

        booktag_temp->author=readstr(f);

        booktag_temp->publisher=readstr(f);

        fread(&booktag_temp->year,sizeof(int), 1,f);

        booktag_temp->language=readstr(f);

        fread(&booktag_temp->pages,sizeof(int),1,f);

        fread(&booktag_temp->price,sizeof(float),1,f);

        printf_booktag(booktag_temp);

        printf_separator();

        printf_colorful("\n\nDigite 1 para continuar a impressao ", ANSI_CYAN);
        rewind(stdin);
        scanf("%d", &cont);
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
   A função recebe o RRN de um registro, procura ele no arquivo e se o encontrar
   marca ele com CHAR_REM para remoçaõ

   @param char filename[] nome do arquivo de dados
   @param int rrn a ser removido
   @return int se encontrado ou não (-1 para erro, 1 para encontrado, 0 para não)
**/


int markrem_booktag(char filename[], int rrn){

    //checamos os parametros
    if (filename == NULL) {
        printf("\n[AVISO] filename precisa ser o nome do arquivo a ser utilizado\n");
    }

    //alocamos a estrutura temporaria e o ponteiro do arquivo
    BOOKTAG_T *booktag_temp = create_booktag();
    FILE *f = fopen_log(filename, "rb");

    //testamos o ponteiro do arquivo, em caso de erro, avisamos
    if(f == NULL) {
        printf("\n[ERRO]Erro na abertura do arquivo %s\n", filename);
        free(booktag_temp);
        return -1;
    }

    rewind(f); //reiniciamos o ponteiro do arquivo (boa prática)
    printf("\nProcurando..\n\n");

    int rrn_rem = 0; // rrn inicial

    //começamos com a primeira leitura
    fread_log(booktag_temp, sizeof(BOOKTAG_T), 1, f);

    //começamos o loop de leitura no arquivo
    do {

        if (rrn_rem == rrn) { //encontramos os registro
            if (booktag_temp->title[0] == '*') { // esse rrn ja esta removido
                printf("\nEsse registro ja foi removido\n");
                return 1;
            } else {
                printf("\nENCONTRADO");

                // imprimimos os arquivo para checagem e marcamos ele com * para remoção
                printf_booktag(booktag_temp); // mostramos sua informação
                booktag_temp->title[0] = CHAR_REM; //marcamos

                if (DEBUG) printf_booktag(booktag_temp); //mostramos em caso de DEBUG

                fclose_log(f); //fechamos para mudar o modo (mais seguro)

                //abrimos como modo de gravação
                f = fopen_log(filename, "r+");
                fseek(f,(sizeof(BOOKTAG_T) * rrn_rem), SEEK_CUR);//pegamos a posição anterior

                fwrite_log(booktag_temp, sizeof(BOOKTAG_T), 1, f); //gravamos

                fclose_log(f);

                free(booktag_temp);

                return 1;
            }
        }
        rrn_rem++; // incrementamos o rrn

    } while(fread_log(booktag_temp, sizeof(BOOKTAG_T), 1, f));
    //continuamos lendo até que o arquivo esteja vazio (a funçaõ feof naõ garante isso)


    printf("\nRegistro não encontrado no arquivo\n");

    fclose_log(f);

    free(booktag_temp);

    return 0; // não encontrou
}

/**
   Função diskrem_booktag() remove fisicamente uma booktag
   A função recebe o nome do arquivo e o substitui por outro que
   não possui as booktags removidas

   @param char filename[] nome do arquivo de dados
   @return int se sucedido ou não (-1 para erro, 1 se o arquivo foi gravado com sucesso)
**/
int diskrem_booktag(char filename[]){

    //verificamos se os parametros estão corretos
    if (filename == NULL) {
        printf("\n[AVISO] filename precisa ser o nome do arquivo a ser utilizado\n");
        return -1;
    }

    //alocamos a estrutura temporaria, e dois ponteiros para arquivo de log e o
    // arquivo temporário
    BOOKTAG_T *booktag_temp = create_booktag();
    FILE *f = fopen_log(filename, "r+");
    FILE *arq = fopen("newfile.dtd","w+"); //arquivo temporário

    if((f == NULL) || (arq == NULL)) { // checamos o retorno e caso de erro, avisamos
        printf("\n[ERRO]Erro na abertura do arquivo %s\n", filename);
        free(booktag_temp);
        fclose(arq);
        return -1;
    }
    //reniciamos o ponteiro do arquivo (boa prática)
    rewind(f);

    //lemos o arquivo binário
    fread_log(booktag_temp, sizeof(BOOKTAG_T), 1, f);
    // loop para ler todo os registros do arquivo
    do{
        //procuramos arquivos que tenham a marca da remoção
        // * no primeiro char do título
        if(booktag_temp->title[0] != '*'){
            fwrite_log(booktag_temp, sizeof(BOOKTAG_T), 1, arq);
        }
    } while(fread_log(booktag_temp, sizeof(BOOKTAG_T), 1, f));

    //fechamos o arquiov
    fclose_log(f);
    remove(filename); // removes o arquivo antigo
    rename("newfile.dtd",filename); // mudamos o nome do arquivo temporario para o original
    diskrem_booktag_log(filename); //logging
    fclose_log(arq);//
    return 1;
}

/**
* recover_year
* recupera regisstro de acordo com ano
* @param FILE* file,
@ @parma int year ano á ser pesquisado
*/
void recover_year (char filename[], int year){
    //verificamos os parametros
    if (filename == NULL || year < 0) {
        printf("\n[AVISO] erro nos parametros");
        return;
    }
    //abrimos o arquivo e testamos
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("[ERRO] erro na abertura do arquivo binario para leitura");
        return;
    }

    //criamos o arquivo temporario e uma flag para checar se checamos
    BOOKTAG_T *aux = create_booktag();
    int found = 0;

    //andamos no arquivo e vamos comparando o ano de todos os registros
    // se os registros forem de mesmo ano digitado
    // eu imprimo e marco a flag,
    // senao, a flag imprimi uma mensagem de que nao foi
    while (fread_log(aux, sizeof(BOOKTAG_T), 1,file)){

        if(aux->year == year){
            printf("\nEncontrado: \n");
            printf_booktag(aux);
            found = 1;
        }
    }

    if(found == 0) printf("\nNenhum livro foi encontrado!\n");
//    sleep(TIME_PRINTF);

    fclose(file);
}
