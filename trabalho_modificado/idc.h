#ifndef IDC_H_
#define IDC_H_

#include "booktag.h"

/*Atenção:
 * estrutura IDXSEC_T funciona da seguinte forma:
 * idx é um ponteiro de arquivo padrão.
 * duplicate é um ponteiro de inteiros que será usado para pesquisa; as duplicações
 * do campo pesquisado serão retornadas neste ponteiro.
 * 
 * Todas as funções implementadas para IDXSEC_T também serão implementadas para o arquivo de
 * índice primário, com suas devidas alterações.
 * */

	typedef struct IDXSEC_T{
		FILE *idxsec;
		int *duplicate;
	}IDXSEC_T;

//funçoes de manipulação do índice primário
	FILE *create_idxprim(char *name);
	int add_idxprim(BOOKTAG_T *book);
	int rm_idxprim(FILE *target, const void *field);
	BOOKTAG_T *search_idxprim(FILE *target, int rrn);


//funções de manipulação do índice secundário
	/*create: cria um IDXSEC_T VAZIO:
	 * 		  arquivo aberto com nome "name"
	 * 		  ponteiro duplicate é NULL
	*/
	IDXSEC_T *create_idxsec(char *name);
	/*add: construirá o índice secundário
	 *     a partir de idxprim, adicionando 
	 *     as referências em target
	 *     
	 *     @return:
	 *     0: caso não possa gerar o conteúdo em target
	 *     1: caso a inserção seja feita.
	*/
	int add_idxsec(FILE *idxprim, IDXSEC_T *target);
	/*rm: remoção das referências relacionadas ao 
	 * field especificado.
	 * @return:
	 * 0: caso não possa ser removido.
	 * 1: caso o valor seja removido.
	*/
	int rm_idxsec(IDXSEC_T *target, char *field);
<<<<<<< HEAD
	void recover_idxsec_byfield(IDXSEC_T *target, char *field);
	BOOKTAG_T *search_idxsec(IDXSEC_T *target,FILE *idxprim);
=======
	/*recover byfield: recupera os dados em relação
	 * ao field especificado.
	*/
	void recorver_idxsec_byfield(IDXSEC_T *target, char *field);
	/* search: busca uma booktag específica usando target e idxprim e um campo específico
	 * @return:
	 * BOOKTAG_T: caso o valor seja encontrado
	 * NULL: caso não encontre.
	*/
	BOOKTAG_T *search_idxsec(IDXSEC_T *target,FILE *idxprim, const void *);
>>>>>>> 5b7d9925981299d0f44dd1e0b367784926e79662
#endif
