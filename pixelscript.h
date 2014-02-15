/*
  Copyright (c) 2007 by Jeff Weisberg
  Author: Jeff Weisberg <jaw @ tcp4me.com>
  Created: 2007-Aug-12 13:10 (EDT)
  Function: 
 
  $Id: pixelscript.h,v 1.4 2008/03/26 01:43:14 jaw Exp $

*/

#include <setjmp.h>

#define PXS_NTYPE_UNDEF		0
#define PXS_NTYPE_INT		1
#define PXS_NTYPE_FLOAT		2
#define PXS_NTYPE_STRING	3
#define PXS_NTYPE_SYMBOL	4
#define PXS_NTYPE_QSYMBOL	5
#define PXS_NTYPE_SUB		6
#define PXS_NTYPE_CODE		7
#define PXS_NTYPE_ARRAY		8
#define PXS_NTYPE_DICT		9
#define PXS_NTYPE_INIT		10	// magic token
#define PXS_NTYPE_MARK		11

#define PXS_TYPE_NAMES	\
	"undef", "int", "float", "string", "symbol", "symbol", \
 	"sub", "code", "array", "dict", "init", "mark"

typedef unsigned int PXS_hash_t;

struct PXS_dict_elem;
struct PXS_env;

#define __PXS_node_common \
	int type; \
	int refcount; \
	struct PXS_node *next;	// sub - singly linked list


typedef struct PXS_node {
    __PXS_node_common;
} PXS_node;

typedef struct PXS_node_int {
    __PXS_node_common;
    int value;
} PXS_node_int;

typedef struct PXS_node_float {
    __PXS_node_common;
    double value;
} PXS_node_float;

typedef struct PXS_node_string {
    __PXS_node_common;
    char *value;
} PXS_node_string;

typedef struct PXS_node_symbol {
    __PXS_node_common;
    char *value;
    PXS_hash_t hash;
} PXS_node_symbol;

typedef struct PXS_node_qsymbol {
    __PXS_node_common;
    char *value;
    PXS_hash_t hash;
} PXS_node_qsymbol;

typedef struct PXS_node_sub {
    __PXS_node_common;
    PXS_node *value;
} PXS_node_sub;

typedef struct PXS_node_code {
    __PXS_node_common;
    void (*value)(struct PXS_env*);
} PXS_node_code;

typedef struct PXS_node_dict {
    __PXS_node_common;
    int size;
    int systemp;	// was created by system, not by user
    struct PXS_dict_elem **elem;
} PXS_node_dict;

typedef struct PXS_node_array {
    __PXS_node_common;
    int size;
    PXS_node **values;
} PXS_node_array;

//################################################################

typedef struct PXS_symbol {
    char *name;
    PXS_hash_t hash;
    struct PXS_symbol *next;
} PXS_symbol;

//################################################################

typedef struct PXS_list_elem {
    PXS_node *node;
    struct PXS_list_elem *next, *prev;
} PXS_list_elem;

typedef struct PXS_list {
    PXS_list_elem *head, *tail;
} PXS_list;

typedef struct PXS_env {
    PXS_list dict;
    PXS_list stack;
    jmp_buf *loop_jmp;
    jmp_buf *error_jmp;
    int trace;
    int xdim, ydim, zdim, xpos, ypos;
    PXS_node_array *r, *g, *b, *a;
    
    // prog
} PXS_env;

//################################################################


void PXS_node_free(PXS_node *);
PXS_node *PXS_node_mark_new(void);
PXS_node *PXS_node_int_new(int);
PXS_node *PXS_node_array_new(int);
PXS_node *PXS_node_float_new(double);
PXS_node *PXS_node_sub_new(PXS_node*);
PXS_node *PXS_node_undef_new(void);
PXS_node *PXS_node_init_new(void);
PXS_node *PXS_node_code_new(void(*)(PXS_env*));


void PXS_env_init(PXS_env*);
void PXS_env_done(PXS_env*);
void PXS_env_add_dict(PXS_env*);
void PXS_env_push(PXS_env*, PXS_node*);
PXS_node *PXS_env_pop(PXS_env*);
void PXS_env_code_def(PXS_env *, char *, void(*)(PXS_env*));

PXS_node *PXS_dict_new(int);
void PXS_dict_put(PXS_node_dict *, char *, PXS_hash_t, PXS_node *);
PXS_node *PXS_dict_get(PXS_node_dict *, char *, PXS_hash_t);
void PXS_dict_delete(PXS_node_dict *, char *, PXS_hash_t);
    
PXS_node *PXS_node_symbol_new(char*, int);
PXS_symbol *PXS_symbol_inter(char *);

void PXS_exec_node(PXS_env *, PXS_node*, int);
void PXS_exec_nodes(PXS_env *, PXS_node*);
void PXS_exec_sub(PXS_env *, PXS_node_sub*);

PXS_node *PXS_node_bind(PXS_env *, PXS_node*);

PXS_node *PXS_compile_string(PXS_env*, char*);
PXS_node *PXS_compile_file(PXS_env*, char*);

void PXS_list_push(PXS_list *, PXS_node *);
PXS_node *PXS_list_pop(PXS_list*);

double PXS_number_to_double(PXS_node *);
int PXS_number_to_int(PXS_node *);

void PXS_error(PXS_env*, PXS_node*, char *, char*, ...);
#define die(x) PXS_error(0,0,__FUNCTION__, x);

void *PXS_malloc(int);
void *PXS_salloc(int);
void *PXS_zalloc(int);

void PXS_imgf_results(PXS_env *, float*, float*, float*, float*);

#define PXS_realloc (void*)realloc
