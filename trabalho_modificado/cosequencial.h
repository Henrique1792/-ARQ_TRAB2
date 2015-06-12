#ifndef COSEQUENCIAL_H_
#define COSEQUENCIAL_H_

#include "idc.h"
#include "list.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


LLIST_T *match(LLIST_T *authorbytes,LLIST_T *publisherbytes);

LLIST_T *merge(LLIST_T *authorbytes,LLIST_T *publisherbytes);

#endif
