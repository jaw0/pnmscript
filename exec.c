/*
  Copyright (c) 2007 by Jeff Weisberg
  Author: Jeff Weisberg <jaw @ tcp4me.com>
  Created: 2007-Aug-13 20:11 (EDT)
  Function: execute code
 
  $Id: exec.c,v 1.3 2008/03/25 22:34:10 jaw Exp $

*/

#include <stdio.h>
#include "pixelscript.h"

// find key in dict stack
static PXS_node *
PXS_env_lookup(PXS_env *env, PXS_node_symbol *key){
    PXS_node *dict;
    PXS_node *n;
    PXS_list_elem *le = env->dict.tail;

    while(le){
	dict = le->node;
	n = PXS_dict_get((PXS_node_dict*)dict, key->value, key->hash);
	if(n) return n;
	le = le->prev;
    }
    return 0;
}

void
PXS_exec_sub(PXS_env *env, PXS_node_sub *node){
    PXS_exec_nodes(env, node->value);
}

void
PXS_exec_code(PXS_env *env, PXS_node_code *node){
    node->value(env);
}

void
trace(PXS_env *env, PXS_node_symbol *node, PXS_node *val){

    fprintf(stderr, "    exec: /%s\tstack: ", node->value );
    PXS_func_pstack(env);
}

// lookup symbol + exec
void
PXS_exec_symbol(PXS_env *env, PXS_node_symbol *node){
    PXS_node *val;

    val = PXS_env_lookup(env, node);
    if( !val ){
	PXS_error(env, (PXS_node*)node, "exec_symbol", "symbol not defined");
    }

    switch(val->type){
      case PXS_NTYPE_CODE:
	    if(env->trace) trace(env, node, val);
	    PXS_exec_code(env, (PXS_node_code*)val);
	    break;
      case PXS_NTYPE_SUB:
	    if(env->trace) trace(env, node, val);
	    PXS_exec_sub(env, (PXS_node_sub*)val);
	    break;
      default:
	    PXS_node_retain(val);
	    PXS_env_push(env, val);
	    break;
    }
}

void
PXS_exec_node(PXS_env *env, PXS_node *node, int fexec){

    switch(node->type){
      case PXS_NTYPE_UNDEF:
	    // RSN - report error
	    PXS_error(env, node, "exec_node", "undefined");
	    break;
      case PXS_NTYPE_SYMBOL:
	    PXS_exec_symbol(env, (PXS_node_symbol*)node);
	    break;
      case PXS_NTYPE_CODE:
	    PXS_exec_code(env, (PXS_node_code*)node);
	    break;
      case PXS_NTYPE_SUB:
	    if( fexec ){
		PXS_exec_sub(env, (PXS_node_sub*)node);
		break;
	    }else{
		// fall through
	    }
      default:
	    PXS_node_retain(node);
	    PXS_env_push(env, node);
	    break;
    }
}

void
PXS_exec_nodes(PXS_env *env, PXS_node *node){

    // follow sub chain
    while(node){
	PXS_exec_node(env, node, 0);
	node = node->next;
    }
}


PXS_node *
PXS_node_bind(PXS_env *env, PXS_node *node){
    PXS_node *ret  = node;
    PXS_node *prev = 0;
    PXS_node *val;
    
    while(node){
	if( node->type == PXS_NTYPE_SYMBOL ){
	    // lookup symbol
	    val = PXS_env_lookup(env, (PXS_node_symbol*)node);
	    // is it code?
	    if( val && val->type == PXS_NTYPE_CODE ){
		// bind it!
		PXS_node_code *cv = (PXS_node_code*)val;
		PXS_node_code *cn = (PXS_node_code*)PXS_node_code_new(cv->value);

		cn->next = node->next;
		node->next = 0;

		if( prev ){
		    prev->next = (PXS_node*)cn;
		}else{
		    ret = (PXS_node*)cn;
		}
		PXS_node_free(node);
		node = (PXS_node*)cn;
	    }
	}

	if( node->type == PXS_NTYPE_SUB ){
	    // sub? recurse
	    PXS_node_sub *sn = (PXS_node_sub*)node;
	    sn->value = PXS_node_bind(env, sn->value);
	}
	
	prev = node;
	node = node->next;
    }

    return ret;
}
