/*
  Copyright (c) 2007 by Jeff Weisberg
  Author: Jeff Weisberg <jaw @ tcp4me.com>
  Created: 2007-Aug-13 19:23 (EDT)
  Function: 
 
  $Id: list.c,v 1.1 2007/09/03 16:52:43 jaw Exp $

*/

#include "pixelscript.h"

void
PXS_list_push(PXS_list *list, PXS_node *node){
    PXS_list_elem *le = PXS_malloc(sizeof(PXS_list_elem));

    le->node = node;
    le->next = 0;
    le->prev = list->tail;

    if( list->tail ){
	list->tail->next = le;
    }
    
    list->tail = le;
	
    if( ! list->head ){
	list->head = le;
    }
}

PXS_node *
PXS_list_pop(PXS_list *list){
    PXS_list_elem *le;
    PXS_node *n;
    
    le = list->tail;

    if( ! le ) return 0;

    if( le->prev ){
	le->prev->next = 0;
    }
    list->tail = le->prev;
    le->prev = 0;

    if( ! list->tail ) list->head = 0;

    n = le->node;
    PXS_free(le);

    return n;
}

