#ifndef COSEC_H_
#define COSEC_H_

#include "stdlib.h"
#include "stdio.h"
#include <string.h>
#include "idc.h"
#include "booktag.h"
#include "debug.h"

/*
   Trabalho de Organizacao de Arquivos - Trabalho 2

   Integrantes:

   Marcos Vinicius Barros L. Andrade Junqueira     numero USP: 8922393
   Rita Raad                                       numero USP: 8061452
   Henrique Fernandes de Mattos Freitas             numero USP: 8937225
   Gustavo Santiago                                numero USP: 8937416

   Descricao do arquivo cosec.h: possui a definicao das estruturas e assinatura das funções (implementadas na cosec.c).

*/


void index_match(char *author, char *publisher);

void index_merge(char *author, char *publisher);


#endif
