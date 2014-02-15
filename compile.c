/*
  Copyright (c) 2007 by Jeff Weisberg
  Author: Jeff Weisberg <jaw @ tcp4me.com>
  Created: 2007-Aug-12 13:57 (EDT)
  Function: read + compile program
 
  $Id: compile.c,v 1.3 2008/03/26 01:43:12 jaw Exp $

*/

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pixelscript.h"

struct input {
    char *data;
    FILE *file;
    int eof;
    int pos;
    int line;
    int ttyp;
    int deep;
    int lc;
};

static PXS_node *compile(PXS_env *, struct input*);

static prompt(struct input *in){
    if( in->deep ){ 
	fprintf(stderr, "%d> ", in->deep);
    }else{
	fprintf(stderr, "%%> ");
    }
		
}

static int
next_char(struct input *in){
    int c;

    if( in->ttyp && in->lc == '\n' ) prompt(in);
    
    if( in->file ){
	c = fgetc(in->file);
	if(c==-1){
	    in->eof = 1;
	    return 0;
	}
    }else{
	c = in->data[in->pos];
	if(!c){
	    in->eof = 1;
	    return 0;
	}
	in->pos ++;
    }
    in->lc = c;
    return c;
}

static void
unget_char(struct input *in, int c){
    if(!c) return;
    if( in->file ){
	ungetc(c, in->file);
    }else{
	in->pos --;
    }
}

static int
next_char_skipws(struct input *in){
    int c;
    
    while(1){
	c = next_char(in);
	if(!c) return 0;	// eof
	switch(c){

	  case '#':
		// eat comment
		while( c != '\n' && c != 0 )
		    c = next_char(in);
		// fall through
	  case '\n':
		// RSN - increment line number
		in->line ++;
	  default:
		if( isspace(c) ) continue;
		
		return c;
	}
    }
}

static int
vallof(int c, int b){
    
    if(c>='0' && c<='9') return c - '0';
    if(c>='a' && c<='z') return c - 'a' + 0xa;
    if(c>='A' && c<='Z') return c - 'A' + 0xA;
    
    return 255;
}

static int
isvalid(int c, int b){
    
    return vallof(c, b) < b;
}


#define INITSTRLEN 24
static PXS_node *
read_string(struct input *in, int term){
    char *buf  = (char*)PXS_salloc(INITSTRLEN);
    int buflen = INITSTRLEN;
    PXS_node_string *n;
    int pos = 0;
    int c, base, val;
    
    while(1){
	c = next_char(in);
	if(!c){
	    die( "unterminated string literal" );
	    return 0;
	}

	if( c == term ){
	    break;
	}

	// escaped chars
	if( c == '\\' ){
	    c = next_char(in);
	    if(!c) die( "unterminated string literal" );
	    
	    switch(c){
	      case '\n':  in->line++;       break;
	      case 'a':     c = '\a';       break;
	      case 'n':     c = '\n';       break;
	      case 'r':     c = '\r';       break;
	      case 'b':     c = '\b';       break;
	      case 't':     c = '\t';       break;
	      case 's':     c = ' ';        break;
	      case 'f':     c = '\f';       break;
	      case 'v':     c = '\v';       break;
	      case 'e':     c = '\033';     break;
	      case '\\':    c = '\\';       break;

	      case '0':
		    base = 8;  goto rnum;
	      case 'x':
	      case 'X':
		    c = next_char(in);
		    if(!c) die( "unterminated string literal" );
		    base = 16; goto rnum;
	      case '1': case '2': case '3':
	      case '4': case '5': case '6':
	      case '7': case '8': case '9':
		    base = 10; goto rnum;
	    rnum:
		    while( isvalid(c, base)){
                        val *= base;
                        val += vallof(c, base);
                        c = next_char(in);
			if(!c) die( "unterminated string literal" );
		    }
		    // unget
		    unget_char(in, c);
		    c = val;
		    break;

	      default:
		    break;
	    }
	}

	// room ?
	if( pos >= buflen - 1 ){
	    buflen += buflen >> 1;  // *= 1.5
	    buf = PXS_realloc(buf, buflen);
	    if(!buf) die( "realloc failed" );
	}
	
	buf[pos++] = c;
    }
    buf[pos] = 0;

    n = PXS_malloc( sizeof(PXS_node_string));
    PXS_node_init((PXS_node*)n, PXS_NTYPE_STRING);
    n->value = buf;

    return (PXS_node*)n;
}

static PXS_node *
read_name_or_number(struct input *in, int qd){
    PXS_node *n;
    int len = 0;
    char *buf  = (char*)PXS_salloc(INITSTRLEN);
    int buflen = INITSTRLEN;
    int base=10, decp=0, negp=0, symp=0;
    int ivalue;
    double dvalue;
    char *eptr=0;
    
    while( 1 ){
	int c = next_char(in);
	if( !c || isspace(c) ) break;
	if( c=='{' || c=='}' ){
	    unget_char(in,c);
	    break;
	}
	if( c=='-')  negp++;
	if( c=='.')  decp++;
	if( c==0x2f) symp++;
	if( c==',')  symp++;
	if( c>=':' && c<='@') symp++;
	if( c>='[' && c<='`') symp++;
	if( c>='G' && c<='Z') symp++;
	if( c>='g' && c<='z') symp++;
	if( c>'z') symp++;

	if( len >= buflen - 1 ){
	    buflen += buflen >> 1;  // *= 1.5
	    buf = PXS_realloc(buf, buflen);
	    if(!buf) die( "realloc failed" );
	}
	
	buf[len++] = c;
	buf[len] = 0;
    }
    if( decp > 1 || negp > 1 ) symp = 1;

    if( len == 4 && !strncmp("INIT", buf, len) ){
	PXS_free(buf);
	return PXS_node_init_new();
    }
    
    if( !qd && !symp ){
	// try as int
	ivalue = strtol(buf, &eptr, 0);
	if( !*eptr ){
	    n = PXS_node_int_new(ivalue);
	    PXS_free(buf);
	    return n;
	}

	// try as float
	dvalue = strtod(buf, &eptr);
	if( !*eptr ){
	    n = PXS_node_float_new(dvalue);
	    PXS_free(buf);
	    return n;
	}
    }

    n = PXS_node_symbol_new(buf, qd);
    PXS_free(buf);
    return n;
}

static PXS_node *
next_node(PXS_env *env, struct input *in){
    PXS_node *n, *sub;
    int c;
    
    c = next_char_skipws(in);
    switch(c){
      case 0:
	    return 0;	// eof
      case '"':
      case '\'':
	    in->deep ++;	    
	    n = read_string(in, c);
	    in->deep --;
	    return n;
	    
      case '{':
	    // new sub
	    in->deep ++;
	    sub = compile(env, in);
	    in->deep --;
	    if( in->eof ){
		die("unterminated { block");
	    }
	    n = PXS_node_sub_new(sub);
	    return n;
      case '}':
	    // end sub
	    return 0;
	    
      case '/':
	    // quoted symbol
	    return read_name_or_number(in, 1);
      default:
	    // name, int, or float
	    unget_char(in, c);
	    return read_name_or_number(in, 0);
    }
}

static PXS_node *
compile(PXS_env *env, struct input* in){
    PXS_node *prog = 0;
    PXS_node *end  = 0;
    PXS_node *n;
    
    while( 1 ){
	n = next_node(env, in);
	// eof ?
	if( !n ) break;

	if( n->type == PXS_NTYPE_INIT ){
	    PXS_node_free(n);
	    n = next_node(env, in);
	    PXS_exec_node(env, n, 1);
	    PXS_node_free(n);
	    continue;
	}
	
	// append PXS_node
	if( end ){
	    end->next = n;
	}else{
	    prog = n;
	}
	
	end = n;
    }

    return prog;
}


// the only public function
PXS_node *
PXS_compile_string(PXS_env *env, char *d){
    PXS_node *n;
    struct input in;

    bzero(&in, sizeof(in));
    in.data = d;
    in.line = 1;
    
    n = compile(env, &in);
    return n;
}

PXS_node *
PXS_compile_file(PXS_env *env, char *file){
    PXS_node *n;
    FILE *f;
    struct input in;

    if( !strcmp(file, "-") ){
	f = stdin;
    }else{
	f = fopen(file, "r");
    }
    if( !f ){
	perror("cannot open file");
	exit(-1);
    }

    bzero(&in, sizeof(in));
    in.file = f;
    in.line = 1;

    n = compile(env, &in);
    fclose(f);
    return n;
}

void
PXS_func_executive(PXS_env *env){
    PXS_node *n;
    struct input in;
    jmp_buf *jsave;
    jmp_buf jb;
   
    bzero(&in, sizeof(in));
    in.file = stdin;
    in.line = 1;
    in.ttyp = 1;

    jsave = env->error_jmp;
    setjmp(jb);
    if( in.lc != '\n' ) prompt(&in);
    env->error_jmp = & jb;
    
    while( 1 ){
	    
	n = next_node(env, &in);
	// eof ?	
	if( !n ) break;
	
	PXS_exec_node(env, n, 0);
	PXS_node_free(n);
    }
    env->error_jmp = jsave;
}
