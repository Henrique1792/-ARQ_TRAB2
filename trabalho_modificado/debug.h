#ifndef __dbg_h__
#define __dbg_h__


// defines
#include <stdio.h>

#define LOG_FILENAME "log.txt" //arquivo de log
#define DEBUG 1 //flag de depura��o

// definicao de cores utilizadas no output
// define-se dois padroes de cores pois o

#define ANSI_RED     "\x1b[31m"
#define ANSI_GREEN   "\x1b[32m"
#define ANSI_YELLOW  "\x1b[33m"
#define ANSI_BLUE    "\x1b[34m"
#define ANSI_MAGENTA "\x1b[35m"
#define ANSI_CYAN    "\x1b[36m"
#define ANSI_WHITE   "\x1b[37m"
#define ANSI_RESET   "\x1b[0m"



void printf_colorful(char string[], char color[]);
void printf_error(char string[]);
void printf_debug();
void printf_separator();


/**
   Funcao fread com sistema de logging
   @param *buffer que ir� armazenar os dados que ser�o lidos
   @param size tamanho dos dados que ser�o lidos
   @param count quantidade de dados que ser�o lidos
   @param stream filehandle/arquivo de onde os dados ser�o lidos
   @return size_t retorno da fun�a� fread
 **/
size_t fread_log(void *buffer, size_t size, size_t count,  FILE *stream); //funcao fread com log

/**
    Funcao fwrite com sistema de logging
    @param *buffer que cont�m o que ser� escrito
    @param size tamanho do buffer a ser escrito
    @param count quantidade de buffers a serem escritos
    @param stream filehandle/arquivo que ser� escrito
    @return size_t retorna o resultado do fwrite.
 **/
size_t fwrite_log(void *buffer,
                  size_t size,
                  size_t count,
                  FILE * stream);

/**
   Fun�a� fgets com sistema de logging
   @param *str ponteiro para o buffer que ir� guardar a informa��o
   @param count quantidade de informa��es que leremos
   @param *stream filehandle/arquivo que ir� ser lido
   @return char retorno da fun��o fgets
 */
char *fgets_log( char* str, int count, FILE *stream );


/**
   Fun��o fopen com funcionalidade logging
   @param filename nome do arquivo que ser� aberto
   @param mode modo de abeertura desse arquivo
   @return FILE* ponteiro para o arquivo aberto
 **/
FILE *fopen_log(const char *filename, const char *mode);


/**
   Fun��o fclose com funcionalidade logging
   @param stream ponteiro para o arquivo que ser� fechado
   @return int retorna 0 ou EOF
 **/

int fclose_log(FILE *stream);



/**
   Fun��o logicrem_booktag com funcionalidade logging
   @param filename: Nome do arquivo que ser� aberto
   @param title: T�tulo da entrada a ser removida
   @param topstack: Topo da pilha, contendo o RRN da �ltima entrada removida ou -1
 **/
void logicrem_booktag_log(const char *filename, const char *title, int topstack, int rrn);

/**
   Fun��o diskrem_booktag com funcionalidade logging
   @param filename: Nome do arquivo que ser� aberto e atualizado
 **/
void diskrem_booktag_log(const char *filename);



char * getline_input(void);
#endif

