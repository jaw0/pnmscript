/*
  Copyright (c) 2007 by Jeff Weisberg
  Author: Jeff Weisberg <jaw @ tcp4me.com>
  Created: 2007-Aug-12 13:32 (EDT)
  Function: general node maint
 
  $Id: node.c,v 1.3 2008/03/25 22:34:14 jaw Exp $

*/

#include <strings.h>

#include "pixelscript.h"

void
PXS_node_init(PXS_node *n, int type){

    n->type     = type;
    n->refcount = 1;
    n->next     = 0;
}

PXS_node *
PXS_node_undef_new(void){
    PXS_node *n = PXS_malloc(sizeof(PXS_node));
    PXS_node_init(n, PXS_NTYPE_UNDEF);
    return n;
}

PXS_node *
PXS_node_mark_new(void){
    PXS_node *n = PXS_malloc(sizeof(PXS_node));
    PXS_node_init(n, PXS_NTYPE_MARK);
    return n;
}

PXS_node *
PXS_node_init_new(void){
    PXS_node *n = PXS_malloc(sizeof(PXS_node));
    PXS_node_init(n, PXS_NTYPE_INIT);
    return n;
}

PXS_node *
PXS_node_int_new(int v){
    PXS_node_int *n = PXS_malloc(sizeof(PXS_node_int));
    PXS_node_init((PXS_node*)n, PXS_NTYPE_INT);
    n->value = v;
    return (PXS_node*)n;
}

PXS_node *
PXS_node_float_new(double v){
    PXS_node_float *n = PXS_malloc(sizeof(PXS_node_float));
    PXS_node_init((PXS_node*)n, PXS_NTYPE_FLOAT);
    n->value = v;
    return (PXS_node*)n;
}

PXS_node *
PXS_node_sub_new(PXS_node *v){
    PXS_node_sub *n = PXS_malloc(sizeof(PXS_node_sub));
    PXS_node_init((PXS_node*)n, PXS_NTYPE_SUB);
    n->value = v;
    return (PXS_node*)n;
}

PXS_node *
PXS_node_code_new(void(*code)(PXS_env*)){
    PXS_node_code *n = PXS_malloc(sizeof(PXS_node_code));
    PXS_node_init((PXS_node*)n, PXS_NTYPE_CODE);
    n->value = code;
    return (PXS_node*)n;
}

PXS_node *
PXS_node_array_new(int size){
    PXS_node_array *n = PXS_malloc(sizeof(PXS_node_array));
    PXS_node_init((PXS_node*)n, PXS_NTYPE_ARRAY);

    n->size = size;
    n->values = PXS_malloc( size * sizeof(PXS_node*) );
    return (PXS_node*)n;
}


void
PXS_node_retain(PXS_node *n){
    n->refcount ++;
}

void
PXS_node_free(PXS_node *n){
    PXS_node *nxt, *tmp;
    PXS_node_sub *nsub;
    PXS_node_string *nstr;
    PXS_node_array *nar;
    int i;
    
    if( --n->refcount ) return;

    switch( n->type ){
      case PXS_NTYPE_UNDEF:
      case PXS_NTYPE_INIT:
      case PXS_NTYPE_INT:
      case PXS_NTYPE_FLOAT:
      case PXS_NTYPE_CODE:
      case PXS_NTYPE_SYMBOL:
      case PXS_NTYPE_QSYMBOL:
      case PXS_NTYPE_MARK:
	    // no extra data to free
	    break;
	    
      case PXS_NTYPE_STRING:
	    nstr = (PXS_node_string*)n;
	    if( nstr->value ) PXS_free(nstr->value);
	    nstr->value = 0;
	    break;
	    
      case PXS_NTYPE_SUB:
	    nsub = (PXS_node_sub*)n;
	    nxt  = nsub->value;
	    while( nxt ){
		tmp = nxt->next;
		nxt->next = 0;
		PXS_node_free(nxt);
		nxt = tmp;
	    }
	    break;
	    
      case PXS_NTYPE_ARRAY:
	    nar = (PXS_node_array*)n;
	    for(i=0; i<nar->size; i++){
		PXS_node_free( nar->values[i] );
	    }
	    if( nar->values ) PXS_free( nar->values );
	    nar->size   = 0;
	    nar->values = 0;
	    break;
	    
      case PXS_NTYPE_DICT:
	    
      default:
	    PXS_error(0, 0, "node_free", "cannot free node type=%d", n->type);
	    break;
    }

    PXS_free(n);
}
