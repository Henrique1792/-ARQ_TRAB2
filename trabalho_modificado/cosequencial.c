
#include "idc.h"
#include "list.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


LLIST_T *match(LLIST_T *authorbytes,LLIST_T *publisherbytes){

	LLIST_T *result = create_list();

	KNOT_T *pauxaut = authorbytes->start;
	KNOT_T *pauxpub = publisherbytes->start;

	while(pauxaut->nxt != NULL && pauxpub->nxt != NULL){

		if (pauxaut->byteoffset < pauxpub->byteoffset){
           pauxaut = pauxaut->nxt;
       }
        else if (pauxaut->byteoffset > pauxpub->byteoffset){
           pauxpub = pauxpub->nxt;
       }
        else{ /* pauxaut->byteoffset == pauxpub->byteoffset */
           add_list(pauxaut->byteoffset,result);
           pauxaut = pauxaut->nxt;
           pauxpub = pauxpub->nxt;
        }

    }

    print_listaux(result);
    return result;
}

LLIST_T *merge(LLIST_T *authorbytes,LLIST_T *publisherbytes){

	LLIST_T *result = create_list();

	KNOT_T *pauxaut = authorbytes->start;
	KNOT_T *pauxpub = publisherbytes->start;

	while(pauxaut->nxt != NULL && pauxpub->nxt != NULL){

		if (pauxaut->byteoffset < pauxpub->byteoffset){
			add_list(pauxaut->byteoffset,result);
            pauxaut = pauxaut->nxt;
        }
        else if (pauxaut->byteoffset > pauxpub->byteoffset){
        	add_list(pauxpub->byteoffset,result);
        	pauxpub = pauxpub->nxt;
        }
        else{ /* pauxaut->byteoffset == pauxpub->byteoffset */
           add_list(pauxaut->byteoffset,result);
           pauxaut = pauxaut->nxt;
           pauxpub = pauxpub->nxt;
        }

    }

    print_listaux(result);
    return result;
}
