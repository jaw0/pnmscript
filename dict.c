/*
  Copyright (c) 2007 by Jeff Weisberg
  Author: Jeff Weisberg <jaw @ tcp4me.com>
  Created: 2007-Aug-12 17:47 (EDT)
  Function: 
 
  $Id: dict.c,v 1.1 2007/09/03 16:52:41 jaw Exp $

*/

#include <string.h>

#include "pixelscript.h"


typedef struct PXS_dict_elem {
    char        *key;	// symbol
    PXS_hash_t  hvalue;
    struct PXS_node *value;

    struct PXS_dict_elem *prev, *next;
} PXS_dict_elem;


static PXS_dict_elem *
locate(PXS_node_dict *dict, char *key, PXS_hash_t h, int b){

    PXS_dict_elem *e = dict->elem[b];

    while(e){
        if( (key == e->key) 
	    ||
	    ( /* e->hvalue == h && */ key[0] == e->key[0] && !strcmp(key, e->key)) ){
            return e;
        }
        e = e->next;
    }
    
    return 0;
}


PXS_node *
PXS_dict_new(int s){
    
    PXS_node_dict *d = PXS_malloc( sizeof(PXS_node_dict) );
    PXS_node_init((PXS_node*)d, PXS_NTYPE_DICT);
    
    if( s <= 0 ) s = 57;
    d->size = s;
    d->elem = PXS_zalloc(s * sizeof(struct PXS_dict_elem*));
    d->systemp = 0;
    
    return (PXS_node*)d;
}

void
PXS_dict_put(PXS_node_dict *dict, char *key, PXS_hash_t h, PXS_node *value){
    PXS_dict_elem *e;
    
    if( ! key ) return;
    
    int b = h % dict->size;

    e = locate(dict, key, h, b);

    if( e ){
	// overwrite elem
	PXS_node_free( e->value );
    }else{
	// insert new elem
	e = PXS_malloc(sizeof(struct PXS_dict_elem));
	e->prev = 0;
	e->next = dict->elem[b];
	if( dict->elem[b] ){
	    dict->elem[b]->prev = e;
	}
	dict->elem[b] = e;
    }
    
    e->hvalue = h;
    e->key    = key;
    e->value  = value;
}

void
PXS_dict_delete(PXS_node_dict *dict, char *key, PXS_hash_t h){
    PXS_dict_elem *e;

    if( ! key ) return;
    int b = h % dict->size;

    e = locate(dict, key, h, b);

    if( e ){
        if( e->next ) e->next->prev = e->prev;
        if( e->prev ) e->prev->next = e->next;

        if( ! e->prev ){
            dict->elem[b] = e->next;
        }

	PXS_node_free( e->value );
	PXS_free(e);
    }
}

PXS_node*
PXS_dict_get(PXS_node_dict *dict, char *key, PXS_hash_t h){
    PXS_dict_elem *e;

    if( ! key ) return;
    int b = h % dict->size;

    e = locate(dict, key, h, b);

    if( e ){
	return e->value;
    }else{
	return 0;
    }
}

