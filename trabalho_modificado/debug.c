#include "debug.h"
#include "booktag.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>


/*
   Trabalho de Organizacao de Arquivos - Trabalho 2

   Integrantes:

   Marcos Vinicius Barros L. Andrade Junqueira     numero USP: 8922393
   Rita Raad                                       numero USP: 8061452
   Henrique Fernandes de Matos Freitas             numero USP: 8937225
   Gustavo Santiago                                numero USP: 8937416

   Descricao do arquivo debug.c: implementacao das funcoes definida no debug.h Possui funcoes de de logging de leitura, escrita, captura
   de input e relacionadas. Lembrando que o logging utilizada o fwrite e fread por baixo, principais funcoes principais do <stdio.h>

   Os cabecalhos de comentarios das funcoes estaos no arquivo debug.h
*/

/**
   Função fopen com funcionalidade logging

   @param filename nome do arquivo que será aberto
   @param mode modo de abeertura desse arquivo
   @return FILE* ponteiro para o arquivo aberto
 **/
FILE *fopen_log(const char *filename, const char *mode) {
    assert(filename != NULL && mode != NULL); //checamos se parametro esta correto


//abrimos arquivo de log e gravamos msg de aviso
    FILE *f_log = fopen(LOG_FILENAME, mode);
    if (f_log == NULL) {
        printf("\n[AVISO]erro na abertura do arquivo de log, continuando\n");
    }

    //abrimos o arquio
    FILE *f = fopen(filename, mode); //abrimos de fato o arquivo

    //grava resultado da operacao
    fprintf(f_log, "\n[ABERTO] abrindo o arquivo %s modo %s\n", filename, mode);

    //fechamos o log e retornamos o ponteiro no nosso arquivo aberto
    fclose_log(f_log);
    return f;
}

/**
   Função fclose com funcionalidade logging

   @param stream ponteiro para o arquivo que será fechado
   @return int retorna 0 ou EOF
 **/

int fclose_log(FILE *stream) {
    assert(stream != NULL); //garantimos parametro correto

    //abrimos arquivo de log e gravamos aviso em caso de erro
    FILE *f_log = fopen(LOG_FILENAME, "a");
    if (f_log == NULL) {
        printf("\n[AVISO]erro na abertura do arquivo de log, continuando\n");
    }

    int i = fclose(stream); //fechamos de fato o stream do parametro

    //verificamos o retorno e gravamos o resultado no arquivo delog

    if (i != EOF) {
        fprintf(f_log, "\n[FECHADO] fechando arquivo de handler %p com sucesso\n", stream);
    } else {
        fprintf(f_log, "\n[ERRO] fechamento o arquivo de handler %p com falha\n", stream);
    }
    fclose(f_log);
    return i;
}

/**
    Funcao fwrite com sistema de logging

    @param *buffer que contém o que será escrito
    @param size tamanho do buffer a ser escrito
    @param count quantidade de buffers a serem escritos
    @param stream filehandle/arquivo que será escrito
    @return size_t retorna o resultado do fwrite.
 **/
size_t fwrite_log(void *buffer,
                  size_t size,
                  size_t count,
                  FILE * stream ) {

    // testando parametros, caso nao, aborta o programa
    assert(buffer != NULL ||  stream != NULL);


    //abrimos o arquivo de log e dermos aviso em caso de erro
    FILE *f = fopen(LOG_FILENAME, "a");
    if (f == NULL) {
        printf("\n[AVISO]erro na abertura do arquivo de log, continuando\n");
    } else {
        fprintf(f, "\n[GRAVANDO] as informações: \n");
    }

    //fazemos de fato a operacao de escrita
    BOOKTAG_T *book = buffer;
    size_t i = fwrite(buffer, size, count, stream); //gravamos

    //mandamos os dados gravados pro log
    fprintf(f, "\n[GRAVOU] retorno %lu Autor: %s Titulo: %s Editora %s Ano %d Idioma: %s Paginas: %d Preco %f\n", i,
            book->author, book->title, book->publisher, book->year, book->language, book->pages,
            book->price);

    fclose_log(f);
    return i;
}
/**
   Funcao fread com sistema de logging

   @param *buffer que irá armazenar os dados que serão lidos
   @param size tamanho dos dados que serão lidos
   @param count quantidade de dados que serão lidos
   @param stream filehandle/arquivo de onde os dados serão lidos
   @return size_t retorno da funçaõ fread
 **/
size_t fread_log(void *buffer, size_t size, size_t count,
              FILE * stream) {

    // testando parametros, caso nao, aborta o programa
    assert(buffer != NULL ||  stream != NULL);

    //abrimos o arquiov de log e dermos aviso em caso de erro
    FILE *f_log = fopen(LOG_FILENAME, "w");
    if (f_log != NULL)
        fprintf(f_log, "\n[LENDO] lendo o arquivo\n");
    else
        fprintf(f_log, "\n[AVISO] erro ao abrir o arquivo de log\n");

    //fazemos o fread e os parametros de fat
    size_t i = fread(buffer, size, count, stream);

    fclose(f_log);

    return i;
}
/**
   Função interna getline_input() reescritura da funçaõ getline (exclusiva para sistemas linux)
   Le uma linha inteira de caracteres vindo do stdin

   @return char *  a string do input
 **/
char * getline_input(void) {


    char * line = malloc(100), * linep = line; // linha que gvai gudar o input e ponteiro para essa linha
    size_t lenmax = 100, len = lenmax; //tamanho maximo e tamanho

    if(line == NULL)
        return NULL;

    //interamos no stdin (teclado) enquanto o usuario digitar
    // se ele digitar espaco ou EOF, paramos o loop
    // pegamos caracter por caracter do stdin/teclado
    // e verificamos se a string formada por eles ate entao esta no tamanho maximo
    // se sim, continuamos, senao retornamos
    for(;;) {
        int c;
        c = fgetc(stdin);
        if(c == EOF)
            break; //para se for final de linha

        if(--len == 0) { // ultrapassamos nosso tamanho
            len = lenmax;
            char * linen = realloc(linep, lenmax *= 2);

            if(linen == NULL) { //erro
                free(linep);
                return NULL;
            }

            line = linen + (line - linep);
            linep = linen; //pegamos o ponteiro da string formada
        }

        if((*line++ = c) == '\n') // verificamos se o usuario digitou \n para para o input
            break;
    }
    *line = '\0';
    return linep;
}

/**
   Funçaõ fgets com sistema de logging

   @param *str ponteiro para o buffer que irá guardar a informação
   @param count quantidade de informações que leremos
   @param *stream filehandle/arquivo que irá ser lido
   @return char retorno da função fgets
 */
char *fgets_log( char* str, int count, FILE *stream ) {
    assert(str != NULL || count > 0 || stream != NULL); // checamos os parametros

    //chamamos um workaround ao fgets que pega uma linha inteiro do input
    //independente se o usuario digitar palavras entre espacos,
    // soh para com o input se ele digitar \n (ENTER)
    str = getline_input();

    // abrimos o arquivo de log e gravamos o conteudo de la
    FILE *f_log = fopen(LOG_FILENAME, "a");
    if (f_log != NULL)
        fprintf(f_log, "\n[INPUT]: pegamos o input  %s \n", str);

    fclose(f_log);
    return str;
}

/**
   Função logicrem_booktag com funcionalidade logging

   @param filename: Nome do arquivo que será aberto
   @param title: Título da entrada a ser removida
 **/
void logicrem_booktag_log(const char *filename, const char *title, int topstack, int rrn){
	assert(filename != NULL || title != NULL); // checando parametros

    // logging
    FILE *f_log = fopen(LOG_FILENAME, "a");

    if (f_log != NULL)
        fprintf(f_log, "\n[REMOÇÃO]: removemos a entrada %s, topstack gravada: %d, novo RRN: %d\n\n", title, topstack, rrn);
    else
        fprintf(f_log, "\n[AVISO]: erro na abertura do arquivo de log");

    fclose(f_log);
}


/**
   Função diskrem_booktag com funcionalidade logging
   @param filename: Nome do arquivo que será aberto e atualizado
 **/
void diskrem_booktag_log(const char *filename){
    assert(filename != NULL); // checando parametros

    // logging
    FILE *f_log = fopen(LOG_FILENAME, "a");
    if (f_log != NULL)
        fprintf(f_log, "[REMOÇÃO]: o arquivo \"%s\" foi atualizado.\n\n", filename);
    else
        fprintf(f_log, "[AVISO] erro na abertura do arquivo de log");

    fclose(f_log);
}

/**
   Funcoa new_booktag_log() grava um aviso no arquivo de log para mostrar o inicio de um novo registro
 **/
void new_booktag_log() {

    FILE *fp = fopen(LOG_FILENAME, "w");
    if(fp != NULL) {
        fprintf(fp, "**********************[Inicio de um novo registro]*********************");
    } else {

    }

    fclose(fp);
}
