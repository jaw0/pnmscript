/*
  Copyright (c) 2007 by Jeff Weisberg
  Author: Jeff Weisberg <jaw @ tcp4me.com>
  Created: 2007-Aug-13 22:26 (EDT)
  Function: 
 
  $Id: func.c,v 1.4 2008/03/26 01:43:12 jaw Exp $

*/

#include <stdarg.h>
#include <stdio.h>
#include "pixelscript.h"

static char *type_names[] = { PXS_TYPE_NAMES };


// copy
// cvs
// begin end dict load store get put

void
PXS_func_pop(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    PXS_node_free(a);
}

void
PXS_func_dup(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);

    PXS_node_retain(a);
    PXS_env_push(env, a);
    PXS_env_push(env, a);
}

void
PXS_func_exch(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    PXS_node *b = PXS_env_pop(env);
    
    PXS_env_push(env, a);
    PXS_env_push(env, b);
}

void
PXS_func_clear(PXS_env *env){

    while(1){
        PXS_node *n = PXS_list_pop(& env->stack);
	if(!n) break;
	PXS_node_free(n);
    }
}

void
PXS_func_index(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    int i = PXS_number_to_int(a);
    PXS_list_elem *le;
    PXS_node *n;
    
    le = env->stack.tail;

    for(; le && i; i--){
	le = le->prev;
    }

    if( !le ){
	PXS_error(env, a, "index", "stack underflow");
    }

    n = le->node;
    PXS_node_free(a);
    PXS_node_retain(n);
    PXS_env_push(env, n);

}

void
PXS_func_roll(PXS_env *env){
    PXS_node *j = PXS_env_pop(env);
    PXS_node *n = PXS_env_pop(env);
    PXS_list_elem *le, *a, *b=0, *c, *d;
    int i;
    int ji = PXS_number_to_int(j);
    int ni = PXS_number_to_int(n);
    
    PXS_node_free(n);
    PXS_node_free(j);

    if( ji < 0 ){
	ji = ni - (-ji % ni);
    }
    ji %= ni;
    if( ji == 0 ) return;

    // find bottom item
    d = le = env->stack.tail;

    for(i=0; le && i<ni-1; i++){
	if( i == ji - 1) c = le;
	if( i == ji ) b = le;
	le = le->prev;
    }
    if(!b) b = le;
    a = le;
    
    if( !le ){
	PXS_error(env, n, "roll", "stack underflow");
    }

    // roll
    if(a->prev) a->prev->next = c;
    else env->stack.head = c;
    c->prev = a->prev;
    b->next = 0;
    env->stack.tail = b;
    d->next = a;
    a->prev = d;
    
}

// put into topmost dict
void
PXS_func_def(PXS_env *env){
    PXS_node *v = PXS_env_pop(env);
    PXS_node_symbol *n = (PXS_node_symbol*)PXS_env_pop(env);
    PXS_node *dict = env->dict.tail->node;

    if( n->type != PXS_NTYPE_SYMBOL && n->type != PXS_NTYPE_QSYMBOL ){
	PXS_error(env, (PXS_node*)n, "def", "not a symbol (%s)", type_names[n->type]);
    }

    PXS_dict_put((PXS_node_dict*)dict, n->value, n->hash, v);
    PXS_node_free((PXS_node *)n);    
}

static void
print_stringlike(PXS_node_string *n, char *fmt){
    fprintf(stderr, fmt, n->value);
}

static void
node_print(PXS_node *a, int how){
    PXS_node_int   *ni;
    PXS_node_float *nf;
    
    switch(a->type){
      case PXS_NTYPE_SYMBOL:
      case PXS_NTYPE_QSYMBOL:
	    print_stringlike((PXS_node_string*)a, how ? "/%s" : "%s");
	    break;
      case PXS_NTYPE_STRING:
	    print_stringlike((PXS_node_string*)a, how ? "\"%s\"" : "%s");
	    break;
      case PXS_NTYPE_INT:
	    ni = (PXS_node_int*)a;
	    fprintf(stderr, "%d", ni->value);
	    break;
      case PXS_NTYPE_FLOAT:
	    nf = (PXS_node_float*)a;
	    fprintf(stderr, "%f", nf->value);
	    break;

      default:
	    fprintf(stderr, "#<%s>", type_names[a->type]);
	    break;
    }
}

void
PXS_func_print(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);

    node_print(a, 0);
    PXS_node_free(a);
}

void
PXS_func_nprint(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);

    node_print(a, 0);
    fprintf(stderr, "\n");
    PXS_node_free(a);
}

void
PXS_func_xprint(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);

    node_print(a, 1);
    fprintf(stderr, "\n");
    PXS_node_free(a);
}

void
PXS_func_pstack(PXS_env *env){
    PXS_list_elem *le = env->stack.tail;
    PXS_node *n;

    while( le ){
	node_print( le->node, 1 );
	fprintf(stderr, " ");
	le = le->prev;
    }
    fprintf(stderr, "\n");

}

void
PXS_error(PXS_env *env, PXS_node *node, char *fnc, char *fmt, ...){
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);

    fprintf(stderr, " in func: %s node: ", fnc);
    if( node ) node_print(node, 1);
    else fprintf(stderr, "#<unknown>");
    if( env ){
	fprintf(stderr, "\nstack: ");
	PXS_func_pstack(env);
    }else{
	fprintf(stderr, "\n");
    }

    if( env->error_jmp ){
	// this will leak memory
	longjmp( * env->error_jmp, 1 );	
    }else{
	exit(-1);
    }
}

void
PXS_func_exec(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);

    if( a->type == PXS_NTYPE_SUB ){
	PXS_exec_sub(env, (PXS_node_sub*)a);
	PXS_node_free(a);
    }else{
	PXS_env_push(env, a);
    }
}

void
PXS_func_if(PXS_env *env){
    PXS_node *code = PXS_env_pop(env);
    PXS_node *cond = PXS_env_pop(env);
    

    if( cond->type != PXS_NTYPE_INT ){
	PXS_error(env, cond, "if", "not an integer");
    }
    if( code->type != PXS_NTYPE_SUB ){
	PXS_error(env, code, "if", "not executable");
    }

    int c = PXS_number_to_int(cond);

    if( c ){
	PXS_exec_sub(env, (PXS_node_sub*)code);
    }

    PXS_node_free(code);
    PXS_node_free(cond);
}

void
PXS_func_ifelse(PXS_env *env){
    PXS_node *elcode = PXS_env_pop(env);
    PXS_node *ifcode = PXS_env_pop(env);
    PXS_node *cond   = PXS_env_pop(env);
    

    if( cond->type != PXS_NTYPE_INT ){
	PXS_error(env, cond, "if", "not an integer");
    }
    if( ifcode->type != PXS_NTYPE_SUB ){
	PXS_error(env, ifcode, "if", "not executable");
    }
    if( elcode->type != PXS_NTYPE_SUB ){
	PXS_error(env, elcode, "if", "not executable");
    }

    int c = PXS_number_to_int(cond);

    if( c ){
	PXS_exec_sub(env, (PXS_node_sub*)ifcode);
    }else{
	PXS_exec_sub(env, (PXS_node_sub*)elcode);
    }	

    PXS_node_free(ifcode);
    PXS_node_free(elcode);
    PXS_node_free(cond);
}

void
PXS_func_break(PXS_env *env){

    if( ! env->loop_jmp ){
	PXS_error(env, 0, "break", "not in a loop");
    }

    longjmp( * env->loop_jmp, 1 );
}

void
PXS_func_loop(PXS_env *env){
    PXS_node *code = PXS_env_pop(env);
    jmp_buf *jsave;
    jmp_buf jb;
    
    if( code->type != PXS_NTYPE_SUB ){
	PXS_error(env, code, "loop", "not executable");
    }

    jsave = env->loop_jmp;
    if(setjmp(jb)){
	// exit called
    }else{
	env->loop_jmp = & jb;
	while(1){
	    PXS_exec_sub(env, (PXS_node_sub*)code);
	}
    }

    env->loop_jmp = jsave;
    PXS_node_free(code);
}

void
PXS_func_repeat(PXS_env *env){
    PXS_node *code = PXS_env_pop(env);
    PXS_node *cnt  = PXS_env_pop(env);
    int n = PXS_number_to_int(cnt);
    jmp_buf *jsave;
    jmp_buf jb;
    
    if( code->type != PXS_NTYPE_SUB ){
	PXS_error(env, code, "loop", "not executable");
    }

    jsave = env->loop_jmp;
    if(setjmp(jb)){
	// exit called
    }else{
	env->loop_jmp = & jb;
	while(n--){
	    PXS_exec_sub(env, (PXS_node_sub*)code);
	}
    }

    env->loop_jmp = jsave;
    PXS_node_free(code);
    PXS_node_free(cnt);
}

void
PXS_func_for(PXS_env *env){
    PXS_node *code = PXS_env_pop(env);
    PXS_node *limit = PXS_env_pop(env);
    PXS_node *incr  = PXS_env_pop(env);
    PXS_node *init  = PXS_env_pop(env);
    PXS_node *val;
    jmp_buf *jsave;
    jmp_buf jb;
    double xinc = PXS_number_to_double(incr);
    int down = 0;
    
    if( code->type != PXS_NTYPE_SUB ){
	PXS_error(env, code, "loop", "not executable");
    }
    if( init->type != PXS_NTYPE_INT && init->type != PXS_NTYPE_FLOAT){
	PXS_error(env, init, "loop", "not a number");
    }
    if( incr->type != PXS_NTYPE_INT && incr->type != PXS_NTYPE_FLOAT){
	PXS_error(env, incr, "loop", "not a number");
    }
    if( limit->type != PXS_NTYPE_INT && limit->type != PXS_NTYPE_FLOAT){
	PXS_error(env, limit, "loop", "not a number");
    }

    if( xinc < 0 ) down = 1;
    
    // init
    if( init->type == PXS_NTYPE_FLOAT || incr->type == PXS_NTYPE_FLOAT ){
	val = PXS_node_float_new( PXS_number_to_double(init) );
    }else{
	val = PXS_node_int_new( PXS_number_to_int(init) );
    }
    
    jsave = env->loop_jmp;
    if(setjmp(jb)){
	// exit called
    }else{
	env->loop_jmp = & jb;
	
	while(1){
	    // compare
	    if( val->type == PXS_NTYPE_FLOAT){
		double vv = PXS_number_to_double(val);
		double lv = PXS_number_to_double(limit);

		if(down){
		    if( vv < lv ) break;
		}else{
		    if( vv > lv ) break;
		}
		PXS_env_push(env, PXS_node_float_new(vv));
	    }else{
		int vv = PXS_number_to_int(val);
		int lv = PXS_number_to_int(limit);

		if(down){
		    if( vv < lv ) break;
		}else{
		    if( vv > lv ) break;
		}
		PXS_env_push(env, PXS_node_int_new(vv));
	    }

	    PXS_exec_sub(env, (PXS_node_sub*)code);

	    // incr
	    if( val->type == PXS_NTYPE_FLOAT){
		PXS_node_float *vv = (PXS_node_float*)val;
		double iv = PXS_number_to_double(incr);
		vv->value += iv;
	    }else{
		PXS_node_int *vv = (PXS_node_int*)val;
		int iv = PXS_number_to_int(incr);
		vv->value += iv;
	    }
	}
    }

    env->loop_jmp = jsave;
    PXS_node_free(val);
    PXS_node_free(code);
    PXS_node_free(init);
    PXS_node_free(incr);
    PXS_node_free(limit);
}

void
PXS_func_undef(PXS_env *env){
    PXS_env_push(env, (PXS_node*)PXS_node_undef_new());
}

void PXS_func_eval(PXS_env *env){
    PXS_node *n = PXS_env_pop(env);
    PXS_node  *prog;
    PXS_node_string *str;
    
    if( n->type != PXS_NTYPE_STRING ){
	PXS_env_push(env, n);
	return;
    }

    str = (PXS_node_string*)n;
    prog = PXS_compile_string(env, str->value);
    if(prog){
	PXS_exec_nodes(env, prog);
    }
}

void PXS_func_bind(PXS_env *env){
    PXS_node *n = PXS_env_pop(env);

    if( n->type == PXS_NTYPE_SUB ){
	PXS_node_sub *sn = (PXS_node_sub*)n;
	sn->value = PXS_node_bind(env, sn->value);
    }
    
    PXS_env_push(env, n);   
}

void
PXS_func_init(PXS_env *env){

#define I(a,b)    PXS_env_code_def(env, a, b)
    
    I("pop",     PXS_func_pop);
    I("dup",     PXS_func_dup);
    I("exch",    PXS_func_exch);
    I("clear",   PXS_func_clear);
    I("index",   PXS_func_index);
    I("roll",    PXS_func_roll);
    I("def",     PXS_func_def);
    I("print",   PXS_func_print);
    I("=",       PXS_func_nprint);
    I("==",      PXS_func_xprint);
    I("pstack",  PXS_func_pstack);
    I("exec",    PXS_func_exec);
    I("if",      PXS_func_if);
    I("ifelse",  PXS_func_ifelse);
    I("loop",    PXS_func_loop);
    I("repeat",  PXS_func_repeat);
    I("for",     PXS_func_for);
    I("break",   PXS_func_break);
    I("undef",   PXS_func_undef);
    I("eval",    PXS_func_eval);
    I("bind",    PXS_func_bind);
    
    extern void PXS_func_executive(PXS_env*);
    I("executive", PXS_func_executive);

#undef I
    
}

