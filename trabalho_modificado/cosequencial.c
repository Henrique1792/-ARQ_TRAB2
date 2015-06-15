
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "idc.h"
#include "list.h"


LLIST_T *match(LLIST_T *authorbytes,LLIST_T *publisherbytes){

	LLIST_T *result = create_list();

	KNOT_T *pauxaut = authorbytes->start;
	KNOT_T *pauxpub = publisherbytes->start;

	while(pauxaut->nxt != NULL && pauxpub->nxt != NULL){

	if (pauxaut->byteoffset < pauxpub->byteoffset){
	  //if(pauxaut!=NULL)
	  // printf("antes: %s \n",pauxaut->byteoffset);
           pauxaut = pauxaut->nxt;
           //if(pauxaut!=NULL)
	  // printf("depois: %s \n",pauxaut->byteoffset);
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

	while(1){

	if (pauxaut->byteoffset < pauxpub->byteoffset){
	    add_list(pauxaut->byteoffset,result);
            pauxaut = pauxaut->nxt;
            if(pauxaut == NULL){
              do{ add_list(pauxpub->byteoffset,result);
                  pauxpub = pauxpub->nxt;
                }while (pauxpub != NULL);
                 break;
            }
        }
        if (pauxaut->byteoffset > pauxpub->byteoffset){
        	add_list(pauxpub->byteoffset,result);
        	pauxpub = pauxpub->nxt;
           if(pauxpub == NULL){
              do{ add_list(pauxaut->byteoffset,result);
                  pauxaut = pauxaut->nxt;
                }while (pauxaut != NULL);
                 break;
           }
        }
        else{ 
           add_list(pauxaut->byteoffset,result);
           pauxaut = pauxaut->nxt;
           pauxpub = pauxpub->nxt;
           if(pauxaut == NULL){
             for(;pauxpub != NULL;pauxpub = pauxpub->nxt)
                add_list(pauxpub->byteoffset,result);
                break;
           }
          if(pauxpub == NULL){
             for(;pauxaut != NULL;pauxaut = pauxaut->nxt)
                add_list(pauxaut->byteoffset,result);
                break;
           }
        }

    }

    print_listaux(result);
    return result;
}
