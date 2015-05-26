#include <stdio.h> // usamos função de input
#include <assert.h> //utilizamos o assert para exigir parametros corretos
#include <unistd.h> // para utilizar o system("clear")
#include <stdlib.h> //utilizamos para malloc/free
#include <string.h> //utilizamos o fgets

#include "debug.h"
#include "booktag.h"
#include "screen.h"

/*
   Trabalho de Organizacao de Arquivos - Trabalho 1

   Integrantes:

   Marcos Vinicius Barros L. Andrade Junqueira     numero USP: 8922393
   Rita Raad                                       numero USP: 8061452
   Henrique Fernandes de Matos Freitas             numero USP: 8937225
   Gustavo                                         numero USP: 8937416

   Descricao do arquivo booktag.c: arquivo que possui a implementacao das funcoes definidas no arquivo booktag.h.
   Possui funcoes relativas a manipulacao, criacao, remocao e checagem dos registros(booktags) no arquivo.

   Toda memoria liberada nas funcoes sao liberadas nelas, exceto a funcao create_booktag.

 */



/**
   @brief Função create_booktag() que aloca uma estrutura do tipo BOOKTAG_T
   @return estrutura alocada
 **/
BOOKTAG_T *create_booktag() {

    //alocamos o novo livro
    BOOKTAG_T *book = malloc(sizeof(BOOKTAG_T)); // criamos a variavel


    //abrimos arquivo de log, caso de erro, mandamos msg
    FILE *fp = fopen_log(LOG_FILENAME, "a");


    if (fp != NULL) { // em caso de sucesso avisamos no arquivo
        fprintf(fp, "********************************** Registro novo ******************************\n\n");
    } else
        fprintf(fp, "\n\n[AVISO] erro na abertura/escrita do arquivo de log\n\n");

    int i; //contador

    //preechemos todos valores pois o gcc por alguma razão utiliza o mesmo espaço de memória de outro registro alocado anteriormente, logo quando alocamos
    // podemos encontrar sujeiras(como informações dos registros anteriores)
    for (i = 0; i < TITLE_MAX; i++) { //titulo
        if (book->title[i]) {
            book->title[i] = ' ';
        }
    }

    for (i = 0; i < AUTHOR_MAX; i++) { // autor
        if (book->author[i]) {
            book->author[i] = ' ';
        }
    }

    for (i = 0; i < PUB_MAX; i++) { // editora
        if (book->publisher[i]) {
            book->publisher[i] = ' ';
        }
    }

    for (i = 0; i < LANG_MAX; i++) { //idioma
        if (book->language[i]) {
            book->language[i] = ' ';
        }
    }

    return book;
}

/**
   @brief Função free_booktag() Libera uma booktag da memória
   @param BOOKTAG_T *booktag que será liberada
**/
void free_booktag(BOOKTAG_T *booktag) {
    if (booktag != NULL) { // verificamos parametro e os liberamos da memória
        free(booktag);
    }
}

/**
   Funcao interna search_last_removed que retorna o rrn de um registro marcado para remocao
   @param char filename[] nome do arquivo de dados;
   @return o rrn encontrado
 **/
int search_last_removed(char filename[]){
	//garantimos os paramêtros corretos para evitar erros no arquivo de dados
	assert(filename != NULL);

	int rrn = -1;
	FILE *f = fopen_log(filename,"r");
	if (f == NULL) {
        return -1;
    }
    BOOKTAG_T *booktag_temp = create_booktag();
    do{
    	if(booktag_temp->title[0] == '*'){
    		free(booktag_temp);
    		fclose_log(f);
    		return rrn;
    	}
    	rrn++;
    }while(fread_log(booktag_temp, sizeof(BOOKTAG_T), 1, f));

    free(booktag_temp);
    fclose_log(f);
    return -1;
}

/**
   @brief Função write_booktags() recebe um booktag que irá ser gravado em um arquivo

   @param BOOKTAG_T *booktag que irá ser gravada no arquivo
   @param char filename[] nome do arquivo que será gravado
   @param int n número de booktags que serão gravadas no filename
   @return int
 **/
void write_booktags(BOOKTAG_T *booktag, char filename[], int n) {

    //garantimos os paramêtros corretos para evitar erros no arquivo de dados
    assert(booktag != NULL || filename != NULL);

    int rrn = search_last_removed(filename);

    //abrimos o arquivo de log e em caso de erro, gravamos aviso nele
    FILE *f;
    if(rrn == -1){
    	f = fopen_log(filename, "a");
    }
    else{
    	f = fopen_log(filename, "r+");
    }

    if (f == NULL) {
        printf("\n[ERRO] erro na abertura do arquivo\n");
        return;
    }

    if(rrn != -1){
    	//reiniciamos o ponteiro no arquivo (boa pratica)
    	rewind(f);
    	fseek(f,(sizeof(BOOKTAG_T) * rrn), SEEK_CUR);
    }

    // gravamooso booktag no arquivo
    fwrite_log(booktag, sizeof(BOOKTAG_T), 1, f);

    printf("\n\nGravação com sucesso\n\n");
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
BOOKTAG_T *read_booktag(char filename[]){

    //verificamos o parametro, em caso de erro avisamos
    if (filename == NULL) {
        printf("\n[AVISO] filename precisa ser o nome do arquivo a ser utilizado\n");
        return NULL;
    }

    //alocamos a estrutura temporária e o ponteiro para o arquivo
    BOOKTAG_T *booktag_temp = create_booktag();
    FILE *f = fopen_log(filename, "rb");

    //verificamos o ponteiro para o arquivo, em caso de erro avisamos
    if(f == NULL) {
        printf("\n[ERRO]Erro na abertura do arquivo %s\n", filename);
        free(booktag_temp);
        return NULL;
    }
    //reiniciamos o ponteiro do arquivo (boa prática)
    rewind(f);

    //lemos uma booktag do arquivo
    fread_log(booktag_temp, sizeof(BOOKTAG_T), 1, f);

    //fechamos e retornamos
    fclose_log(f);
    return booktag_temp;
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
    FILE *f = fopen_log(filename, "rb");

    //verificamos o ponteiro de arquivo, em caso de erro, avisamos
    if(f == NULL) {
        printf("\n[ERRO]Erro na abertura do arquivo %s\n", filename);
        free(booktag_temp);
        return;
    }

    rewind(f); //reiniciamos posiçaõ no arquivo por precaução

    int rrn = 0; // rrn inicial


    //lemos uma estrutura do arquivo e mostramos seu RRN e contéudo
    while(fread_log(booktag_temp, sizeof(BOOKTAG_T), 1, f)) {
        printf("\nRRN: %d", rrn);
        printf_booktag(booktag_temp);
        rrn++;
    }

    //liberamos memória e fechamos o ponteiro
    free_booktag(booktag_temp);
    fclose_log(f);
    return;
}
/**
   Funcao read_booktag_list_one() que le um registro por vez do arquivo e pede input para continuar a
   leitura
   @param char filename[] nome do arquivo a ser lido os dados
 **/
void read_booktag_list_one(char filename[]) {

    //verificamos se o parametro está ok
    if (filename == NULL) {
        printf("\n[AVISO] filename precisa ser o nome do arquivo a ser utilizado\n");
        return;
    }

    //alocamos a estrutura temporária e o ponteiro para função
    BOOKTAG_T *booktag_temp = create_booktag();
    FILE *f = fopen_log(filename, "rb");

    //verificamos o ponteiro, em caso de erro avisamos
    if(f == NULL) {
        printf("\n[ERRO]Erro na abertura do arquivo %s\n", filename);
        free(booktag_temp);
        return;
    }

    rewind(f); //reiniciamos posiçaõ no arquivo por precaução

    int rrn = 0;
    int opt = 1;

    //lemos uma estrutura do arquivo e mostramos seu RRN e contéudo
    while(fread_log(booktag_temp, sizeof(BOOKTAG_T), 1, f) && opt == 1) {
        printf("\nRRN: %d", rrn); // imprimos rrn
        printf_booktag(booktag_temp); //imprimos conteudo da estrutura
        rrn++; // incrementamos rrn

        //para continuar com o relatório
        printf("\nDigite 1 para continuar com a visualização do registro: ");
        scanf("%d", &opt);
    }

    //liberamos tudo e limpamos
    free_booktag(booktag_temp);
    fclose_log(f);
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
* recover_rrn
* recupera o registro de acordo com o RRN
* @param FILE* file,
* @param int rrn á ser pesquisado
*/
void recover_rrn (char filename[], int rrn){
    //verificamos os parametros
    if (filename == NULL || rrn < 0) {
        printf("\n[AVISO] erro nos parametros");
        return;
    }
    //abrimos o arquivo e testamos
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("[ERRO] erro na abertura do arquivo binario para leitura");
        return;
    }

    BOOKTAG_T *aux = create_booktag(); //pegamos o booktag temporario
    //irmos ate o rrns
    fseek(fp,sizeof(BOOKTAG_T)*rrn, SEEK_SET);
    // lemos essa posicao e lemos o que foi carregado
    int i = fread_log(aux, sizeof(BOOKTAG_T), 1,fp);

    if (i != 0 && aux->title[0] == '*' ) { // ele ta marcado para remocao
        printf("\n Registro removido \n");
        free(aux);
        fclose(fp);
        return;

    } else {

        //caso ele exista, imprimimos o conteudo dele
        if (i != 0 )  {
            printf_booktag(aux);
            free(aux);
            fclose(fp);
            return;
        }

        //estamos pegando um rrn nao valido/inexiste
        else if (strlen(aux->title) == 0 || strlen(aux->author) == 0 || strlen(aux->publisher)) {
            printf("\nRegistro nao encontrado\n\n");
            sleep(TIME_PRINTF);
        }
    }


    //liberamos e fechamos o arquivo
    free(aux);
    fclose(fp);
    return;
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
    sleep(TIME_PRINTF);

    fclose(file);
}
