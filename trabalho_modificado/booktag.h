#ifndef BOOKTAG_H_
#define BOOKTAG_H_
#include <stdio.h>

/*
   Trabalho de Organizacao de Arquivos - Trabalho 2

   Integrantes:

   Marcos Vinicius Barros L. Andrade Junqueira     numero USP: 8922393
   Rita Raad                                       numero USP: 8061452
   Henrique Fernandes de Mattos Freitas             numero USP: 8937225
   Gustavo Santiago                                numero USP: 8937416

   Descricao do arquivo booktag.h: possui a definicao da estrutura e assinatura das funcoes (implementadas na booktag.c). Alem disso,
   possui os defines do tamanho de dados (mais detalhes na documentacao impressa ou em formato pdf).

*/

#define TITLE_MAX 80  /*!< quantidade maxima de caracteres do titulo  */
#define AUTHOR_MAX 40 /*!< quantidade maxima de caracteres do autor  */
#define PUB_MAX 20 /*!< quantidade maxima de caracteres da editora  */
#define LANG_MAX 30 /*!< quantidade maxima de caracteres da do idioma  */
#define BUFFER_MAX 1024  /*!< tamanho maximo do buffer  */
#define DATAFILE_PATH "data.bin"   /*!< nome do arquivo de dados  */
#define TOPSTACK_PATH "top.stack" /*!< nome do arquivo da topstack  */
#define IDXLISTAUT_PATH "idx_listaut.idx" /*!< nome do arquivo de lista invertida  */
#define IDXLISTPUB_PATH "idx_listpub.idx" /*!< nome do arquivo de lista invertida  */
#define IDXSECAUT_PATH "idx_author.idx" /*!< nome do arquivo de indice secundario  */
#define IDXSECPUB_PATH "idx_publisher.idx" /*!< nome do arquivo de indice secundario  */
#define CHAR_REM '*'    /*!< caracter de remocao  */
#define DELIM '|' /*!< caracter delimitador  */

/**
   @brief Estrutura que representa as booktags/informacoes do livro no nosso programa
 **/
typedef struct{
	char *title;//titulo
	char *author; //autor
	char *publisher;//editora
	int year;//ano
	char *language;//idioma
	int pages;//qtd paginas
	float price; // preco
} BOOKTAG_T;

//
// Prototipo das funções implementadas no booktag.c
//

/**
   @brief Função create_booktag() que aloca uma estrutura do tipo BOOKTAG_T
   @return estrutura alocada
 **/
BOOKTAG_T* create_booktag();

/**
   @brief Função free_booktag() Libera uma booktag da memória
   @param BOOKTAG_T *booktag que será liberada
**/
void free_booktag(BOOKTAG_T *booktag);


/**
   @brief Função write_booktags() recebe um booktag que irá ser gravado em um arquivo

   @param BOOKTAG_T *booktag que irá ser gravada no arquivo
   @param char filename[] nome do arquivo que será gravado
   @param int n número de booktags que serão gravadas no filename
   @return int
 **/
void write_booktags(BOOKTAG_T *booktag, char filename[]);

/**
   Funcao read_booktag() lê do arquivo de nome filename e retorna o primeiro booktag

   @param char filename[] nome do arquivo a ser lido
 **/
BOOKTAG_T *read_booktag(FILE *f);

/**
   Funcao read_booktag_list() que le uma lista de booktags de um arquivo e os imprimi

   @param char filename[] nome do arquivo a ser lido
 **/
void read_booktag_list(char filename[]);

/**
   Função markrem_booktag() remove logicamente uma booktag
   A função recebe o RRN de um registro, procura ele no arquivo e se o encontrar
   marca ele com CHAR_REM para remoçaõ

   @param char filename[] nome do arquivo de dados
   @param int rrn a ser removido
   @return int se encontrado ou não (-1 para erro, 1 para encontrado, 0 para não)
**/
int  markrem_booktag(char filename[], char *field, int topstack);

/**
   Função diskrem_booktag() remove fisicamente uma booktag
   A função recebe o nome do arquivo e o substitui por outro que
   não possui as booktags removidas

   @param char filename[] nome do arquivo de dados
   @return int se sucedido ou não (-1 para erro, 1 se o arquivo foi gravado com sucesso)
**/
int  diskrem_booktag(char filename[]);

/**
* recover_year
* recupera regisstro de acordo com ano
* @param FILE* file,
@ @parma int year ano á ser pesquisado
*/
void recover_year (char filename[],  int year);

void printf_booktag(BOOKTAG_T *booktag);

BOOKTAG_T *get_booktag(FILE *f, int *tam);

/**
   Funcao read_booktag_list_one() que le uma lista de booktags imprimindo um registor por vez

   @param char filename[] nome do arquivo a ser lido
 **/
void read_booktag_list_one(char filename[]);

char *readstr(FILE *target);



#endif
