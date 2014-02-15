/*
  Copyright (c) 2007 by Jeff Weisberg
  Author: Jeff Weisberg <jaw @ tcp4me.com>
  Created: 2007-Aug-13 21:51 (EDT)
  Function: maintain the environment: stack + dict stack
 
  $Id: env.c,v 1.3 2008/03/25 22:34:10 jaw Exp $

*/

#include "pixelscript.h"

extern void PXS_math_init(PXS_env*);
extern void PXS_func_init(PXS_env*);
extern void PXS_imgf_init(PXS_env*);

// RSN - init code
char *init_code = "/version \"PIXELSCRIPT 1.0\" def";


// add another dict
void
PXS_env_add_dict(PXS_env *env){

    PXS_node *dict = PXS_dict_new(613);
    ((PXS_node_dict*)dict)->systemp = 1;
    PXS_list_push( &env->dict, dict );
}

// initailize the system
void
PXS_env_init(PXS_env *env){
    PXS_node *init;

    bzero(env, sizeof(struct PXS_env));
    
    // install system dict
    PXS_env_add_dict(env);

    // RSN - add builtins to dict
    PXS_math_init(env);
    PXS_func_init(env);
    PXS_array_init(env);
    PXS_imgf_init(env);
    // ...

    // load init script
    init = PXS_compile_string(env, init_code);
    PXS_exec_nodes(env, init);
    
    // install user dict
    PXS_env_add_dict(env);
}

void
PXS_env_done(PXS_env *env){

}

PXS_node *
PXS_env_pop(PXS_env *env){
    PXS_node *n = PXS_list_pop(& env->stack);

    if( !n ){
	PXS_error(env, 0, "env/pop", "stack underflow");
    }

    return n;
}

void
PXS_env_push(PXS_env *env, PXS_node *node){
    PXS_list_push(& env->stack, node);
}

// install system code into dict
void
PXS_env_code_def(PXS_env *env, char *name, void(*code)(PXS_env*)){
    
    PXS_node *dict = env->dict.tail->node;
    
    // create symbol
    PXS_symbol *sym = PXS_symbol_inter(name);
    
    // create code
    PXS_node_code *cn = (PXS_node_code*)PXS_node_code_new(code);
    
    // put into topmost dict
    PXS_dict_put((PXS_node_dict*)dict, sym->name, sym->hash, (PXS_node*)cn);
    
}
