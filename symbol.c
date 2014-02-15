/*
  Copyright (c) 2007 by Jeff Weisberg
  Author: Jeff Weisberg <jaw @ tcp4me.com>
  Created: 2007-Aug-12 22:45 (EDT)
  Function: 
 
  $Id: symbol.c,v 1.1 2007/09/03 16:52:44 jaw Exp $

*/

#include <string.h>

#include "pixelscript.h"


PXS_symbol **byhash = 0;

#define SYMHASHSIZE     397

static void
symbol_init(){

    byhash = (PXS_symbol**)PXS_zalloc(SYMHASHSIZE * sizeof( PXS_symbol* ));
}

PXS_hash_t
PXS_hash(const char* str){
   unsigned int hash = 5381;
   int i;

   while(*str){
      hash = ((hash << 5) + hash) + (*str);
      str++;
   }

   return (hash & 0x7FFFFFFF);
}

PXS_symbol *
PXS_symbol_inter(char *p){
    PXS_hash_t h;
    int b;
    PXS_symbol *s, *ns;
    
    if( !byhash ) symbol_init();
    h = PXS_hash(p);
    b = h % SYMHASHSIZE;
    s = byhash[b];

    for(;s;s=s->next){
	if( s->hash == h && !strcmp(p, s->name)){
	    // symbol exists
	    return s;
	}
    }
    
    // into hash table
    ns = (PXS_symbol*)PXS_malloc(sizeof(PXS_symbol));
    ns->next  = byhash[b];
    ns->name  = strdup(p);
    ns->hash  = h;
    byhash[b] = ns;

    return ns;
    
}

PXS_node *
PXS_node_symbol_new(char *p, int qd){
    PXS_symbol *s;
    
    PXS_node_symbol *n = PXS_malloc( sizeof(PXS_node_symbol) );
    PXS_node_init((PXS_node*)n, qd ? PXS_NTYPE_QSYMBOL : PXS_NTYPE_SYMBOL );

    s = PXS_symbol_inter(p);
    
    n->value = s->name;
    n->hash  = s->hash;
    return (PXS_node*)n;
}

