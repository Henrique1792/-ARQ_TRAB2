#ifndef BOOKTAG_H_
#define BOOKTAG_H_

#include <stdio.h>

/*
   Trabalho de Organizacao de Arquivos - Trabalho 1

   Integrantes:

   Marcos Vinicius Barros L. Andrade Junqueira     numero USP: 8922393
   Rita Raad                                       numero USP: 8061452
   Henrique Fernandes de Matos Freitas             numero USP: 8937225
   Gustavo                                         numero USP: 8937416

   Descricao do arquivo booktag.h: possui a definicao da estrutura e assinatura das funcoes (implementadas na booktag.c). Alem disso,
   possui os defines do tamanho de dados (mais detalhes na documentacao impressa ou em formato pdf).

*/


#define TITLE_MAX 80  /*!< quantidade maxima de caracteres do titulo  */
#define AUTHOR_MAX 40 /*!< quantidade maxima de caracteres do autor  */
#define PUB_MAX 20 /*!< quantidade maxima de caracteres da editora  */
#define LANG_MAX 30 /*!< quantidade maxima de caracteres da do idioma  */
#define BUFFER_MAX 1024  /*!< tamanho maximo do buffer  */
#define DATAFILE_PATH "data.bin"   /*!< nome do arquivo de dados  */
#define CHAR_REM '*'    /*!< caracter de remocao  */

/**
   @brief Estrutura que representa as booktags/informacoes do livro no nosso programa
 **/
typedef struct{
	char title[TITLE_MAX];//titulo
	char author[AUTHOR_MAX]; //autor
	char publisher[PUB_MAX];//editora
	int year;//ano
	char language[LANG_MAX];//idioma
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
void write_booktags(BOOKTAG_T *booktag, char filename[], int n);

/**
   [TODO]
   Função write_booktag_opt() Recebe um booktag que irá ser gravado em um arquivo
   possui otimização na inserção (insere em registros que foram removidos lógicamente)

   @param booktag que irá ser gravada no arquivo
   @param char filename[] nome do arquivo que será gravado
 **/
void write_booktag_opt(BOOKTAG_T *booktag, char filename[]);

/**
   Funcao read_booktag() lê do arquivo de nome filename e retorna o primeiro booktag

   @param char filename[] nome do arquivo a ser lido
 **/
BOOKTAG_T *read_booktag(char filename[]);

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
int  markrem_booktag(char filename[], int rrn);

/**
   Função diskrem_booktag() remove fisicamente uma booktag
   A função recebe o nome do arquivo e o substitui por outro que
   não possui as booktags removidas

   @param char filename[] nome do arquivo de dados
   @return int se sucedido ou não (-1 para erro, 1 se o arquivo foi gravado com sucesso)
**/
int  diskrem_booktag(char filename[]);

/**
   Função read_booktag_list_one() que le um registro por vez do arquivo e pede input para
   continuar aleitura
   @param char filename[] nome do arquivo a ser lido os dados
 **/
void read_booktag_list_one(char filename[]);


/**
* recover_rrn
* recupera o registro de acordo com o RRN
* @param FILE* file,
* @param int rrn á ser pesquisado
*/
void recover_rrn (char filename[],  int rrn);

/**
* recover_year
* recupera regisstro de acordo com ano
* @param FILE* file,
@ @parma int year ano á ser pesquisado
*/
void recover_year (char filename[],  int year);


#endif
