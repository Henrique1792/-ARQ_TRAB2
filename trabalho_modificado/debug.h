#ifndef __dbg_h__
#define __dbg_h__


// defines
#include <stdio.h>

#define LOG_FILENAME "log.txt" //arquivo de log
#define DEBUG 1 //flag de depuração


/**
   Funcao fread com sistema de logging

   @param *buffer que irá armazenar os dados que serão lidos
   @param size tamanho dos dados que serão lidos
   @param count quantidade de dados que serão lidos
   @param stream filehandle/arquivo de onde os dados serão lidos
   @return size_t retorno da funçaõ fread
 **/
size_t fread_log(void *buffer, size_t size, size_t count,  FILE *stream); //funcao fread com log

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
                  FILE * stream);

/**
   Funçaõ fgets com sistema de logging

   @param *str ponteiro para o buffer que irá guardar a informação
   @param count quantidade de informações que leremos
   @param *stream filehandle/arquivo que irá ser lido
   @return char retorno da função fgets
 */
char *fgets_log( char* str, int count, FILE *stream );


/**
   Função fopen com funcionalidade logging

   @param filename nome do arquivo que será aberto
   @param mode modo de abeertura desse arquivo
   @return FILE* ponteiro para o arquivo aberto
 **/
FILE *fopen_log(const char *filename, const char *mode);


/**
   Função fclose com funcionalidade logging

   @param stream ponteiro para o arquivo que será fechado
   @return int retorna 0 ou EOF
 **/

int fclose_log(FILE *stream);



/**
   Função logicrem_booktag com funcionalidade logging

   @param filename: Nome do arquivo que será aberto
   @param title: Título da entrada a ser removida
   @param topstack: Topo da pilha, contendo o RRN da última entrada removida ou -1
 **/
void logicrem_booktag_log(const char *filename, const char *title, int topstack, int rrn);

/**
   Função diskrem_booktag com funcionalidade logging
   @param filename: Nome do arquivo que será aberto e atualizado
 **/
void diskrem_booktag_log(const char *filename);


#endif
