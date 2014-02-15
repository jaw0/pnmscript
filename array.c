 /*
  Copyright (c) 2008 by Jeff Weisberg
  Author: Jeff Weisberg <jaw @ tcp4me.com>
  Created: 2008-Mar-25 16:36 (EDT)
  Function: 
 
  $Id: array.c,v 1.2 2008/03/26 01:43:12 jaw Exp $

*/

#include "pixelscript.h"

void
PXS_array_array(PXS_env *env){
    PXS_node *nsz = PXS_env_pop(env);
    PXS_node_array *na;
    int i;
    int size = PXS_number_to_int(nsz);

    na = (PXS_node_array*)PXS_node_array_new(size);
    for(i=0; i<size; i++){
	na->values[i] = PXS_node_undef_new();
    }
    
    PXS_node_free(nsz);
    PXS_env_push(env, (PXS_node*)na );
}

void
PXS_array_mark(PXS_env *env){

    PXS_env_push(env, PXS_node_mark_new());
}

void
PXS_array_create_to_mark(PXS_env *env){

    PXS_list_elem *le = env->stack.tail;
    PXS_node *n;
    PXS_node_array *na;
    int size = 0, i;
    
    while( le ){
	if( le->node->type == PXS_NTYPE_MARK ) break;
	size ++;
	le = le->prev;
    }

    if( !le ){
	PXS_error(env, 0, "]", "stack underflow. mark not found.");
    }

    na = (PXS_node_array*)PXS_node_array_new(size);
    
    // fill
    for(i=0; i<size; i++){
	n = PXS_env_pop(env);
	na->values[size - i - 1] = n;
    }

    // remove mark
    n = PXS_env_pop(env);
    PXS_node_free(n);

    PXS_env_push(env, (PXS_node*)na );
}

void
PXS_array_length(PXS_env *env){
    PXS_node *nna  = PXS_env_pop(env);
    PXS_node_array *na = (PXS_node_array*)nna;

    if( nna->type != PXS_NTYPE_ARRAY ){
	PXS_error(env, nna, "get", "not an array");
    }

    PXS_env_push(env, PXS_node_int_new(na->size));
    PXS_node_free(nna);
}

void
PXS_array_get(PXS_env *env){
    PXS_node *nidx = PXS_env_pop(env);
    PXS_node *nna  = PXS_env_pop(env);
    PXS_node *r;
    int i = PXS_number_to_int(nidx);
    PXS_node_array *na = (PXS_node_array*)nna;

    // validate
    if( nna->type != PXS_NTYPE_ARRAY ){
	PXS_error(env, nna, "get", "not an array");
    }
    if( i >= na->size || i < 0 ){
	PXS_error(env, nidx, "get", "index out of bounds");
    }

    r = na->values[i];

    PXS_node_retain(r);
    PXS_node_free(nidx);
    PXS_node_free(nna);
    PXS_env_push(env, r);
}

void
PXS_array_put(PXS_env *env){
    PXS_node *nval = PXS_env_pop(env);
    PXS_node *nidx = PXS_env_pop(env);
    PXS_node *nna  = PXS_env_pop(env);
    int i = PXS_number_to_int(nidx);
    PXS_node_array *na = (PXS_node_array*)nna;

    // validate
    if( nna->type != PXS_NTYPE_ARRAY ){
	PXS_error(env, nna, "put", "not an array");
    }
    if( i >= na->size || i < 0 ){
	PXS_error(env, nidx, "put", "index out of bounds");
    }

    PXS_node_free( na->values[i] );
    na->values[i] = nval;

    PXS_node_free(nidx);
    PXS_node_free(nna);
}

void
PXS_array_forall(PXS_env *env){
    PXS_node *code = PXS_env_pop(env);
    PXS_node *nna  = PXS_env_pop(env);
    PXS_node *val;
    jmp_buf *jsave;
    jmp_buf jb;
    int i;
    PXS_node_array *na = (PXS_node_array*)nna;
    
    if( code->type != PXS_NTYPE_SUB ){
	PXS_error(env, code, "forall", "not executable");
    }
    if( nna->type != PXS_NTYPE_ARRAY ){
	PXS_error(env, nna, "forall", "not an array");
    }
    
        
    jsave = env->loop_jmp;
    if(setjmp(jb)){
	// exit called
    }else{
	env->loop_jmp = & jb;

	for(i=0; i<na->size; i++){
	    val = na->values[i];
	    PXS_node_retain(val);
	    PXS_env_push(env, val);

	    PXS_exec_sub(env, (PXS_node_sub*)code);
	}
    }

    env->loop_jmp = jsave;
    PXS_node_free(code);
    PXS_node_free(nna);
}

// [ cond then cond then cond then ... else ] case
void
PXS_array_case(PXS_env *env){
    PXS_node *nna = PXS_env_pop(env);
    PXS_node_array *na = (PXS_node_array*)nna;
    int i;
    
    if( nna->type != PXS_NTYPE_ARRAY ){
	PXS_error(env, nna, "forall", "not an array");
    }

    for(i=0; i<na->size; i+=2){
	PXS_node *code, *cond;

	if( i == na->size - 1 ){
	    // else clause
	    cond = 0;
	    code = na->values[i];
	}else{
	    cond = na->values[i];
	    code = na->values[i+1];
	}
	
	if( cond && cond->type != PXS_NTYPE_INT ){
	    PXS_error(env, cond, "case", "not an integer");
	}
	if( code->type != PXS_NTYPE_SUB ){
	    PXS_error(env, code, "case", "not executable");
	}

	int c = cond ? PXS_number_to_int(cond) : 1;

	if( c ){
	    PXS_exec_sub(env, (PXS_node_sub*)code);
	    break;
	}
    }
    
    PXS_node_free(nna);
}

void
PXS_array_init(PXS_env *env){

#define I(a,b)    PXS_env_code_def(env, a, b)
    
    I("[",       PXS_array_mark);
    I("]",       PXS_array_create_to_mark);
    I("array",   PXS_array_array);
    I("length",  PXS_array_length);
    I("get",     PXS_array_get);
    I("put",     PXS_array_put);
    I("forall",  PXS_array_forall);
    I("case",    PXS_array_case);

#undef I
}

