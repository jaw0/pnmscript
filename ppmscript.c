/*
  Copyright (c) 2007 by Jeff Weisberg
  Author: Jeff Weisberg <jaw @ tcp4me.com>
  Created: 2007-Aug-13 23:06 (EDT)
  Function: 
 
  $Id: ppmscript.c,v 1.4 2008/03/26 01:43:14 jaw Exp $

*/

#include <stdlib.h>
#include <shhopt.h>
#include <ppm.h>
#include "pixelscript.h"

PXS_env env;

char *opt_file = 0;
char *opt_expr = 0;
char *opt_lib  = 0;
int help_p = 0;
int shell_p = 0;
int trace_p = 0;

const char usage[] =
"ppmscript [args] [files]\n"
"  -expr code     - specify code to run on cmdline\n"
"  -file script   - read code to run from file\n"
"  -lib script    - read library code from file\n"
"  -trace         - display program debugging trace while running\n"
"  -shell         - launch interactive executive shell\n"
"  -help          - provide help\n";

optEntry opt_e[] = {
    /* short_name, long_name, type, arg, specified, flags */

    { 0, "file",       OPT_STRING, &opt_file, 0, 0 },	// load script from file
    { 0, "expr",       OPT_STRING, &opt_expr, 0, 0 },	// load program from cmd line
    { 0, "lib",        OPT_STRING, &opt_lib,  0, 0 },	// load script library
    { 0, "help",       OPT_FLAG,   &help_p,   0, 0 },
    { 0, "shell",      OPT_FLAG,   &shell_p,  0, 0 },	// interactive shell
    { 0, "trace",      OPT_FLAG,   &trace_p,  0, 0 },	// trace output
    { 0, 0,            OPT_END,    0,         0, 0 },
};
optStruct3 opt = { 0, 1, opt_e }; /* short_p, neg_p, entries */

struct PXS_ppm_input {
    FILE *f;
    int rows, cols, format;
    pixval maxval;
    pixel *irow;
};

int
main(int argc, char **argv){
    int rows, cols, depth, row, col, maxval, i;
    pixel *orow;
    int   rb=0, gb=0, bb=0;
    float rf=1, gf=1, bf=1;
    PXS_node *prog;
    struct PXS_ppm_input *input;

    PXS_env_init(&env);
    ppm_init( &argc, argv );
    
    optParseOptions3( &argc, argv, opt, sizeof(opt), 0 );

    if( help_p ){
	fprintf(stderr, usage);
        exit(-1);
    }

    depth = argc > 1 ? (argc - 1) : 1;
    env.trace = trace_p;
    
    env.r = (PXS_node_array*)PXS_node_array_new( depth );
    env.g = (PXS_node_array*)PXS_node_array_new( depth );
    env.b = (PXS_node_array*)PXS_node_array_new( depth );
    env.a = (PXS_node_array*)PXS_node_array_new( depth );
    for(i=0; i<depth; i++){
	env.r->values[i] = PXS_node_undef_new();
	env.g->values[i] = PXS_node_undef_new();
	env.b->values[i] = PXS_node_undef_new();
	env.a->values[i] = PXS_node_undef_new();
    }

    
    // load libs
    if( opt_lib ){
	PXS_node *lib = PXS_compile_file(&env, opt_lib);
	if(lib) PXS_exec_nodes(&env, lib);
    }

    // launch executive
    if( shell_p ){
	struct PXS_node *prog;
	prog = PXS_compile_string(&env, "executive");
	PXS_exec_nodes(&env, prog);
	exit(0);
    }
    
    // load prog
    if( opt_expr ){
	prog = PXS_compile_string(&env, opt_expr);
    }
    else if( opt_file ){
	prog = PXS_compile_file(&env, opt_file);
    }else{
	fprintf(stderr, "must specify expr or file\n");
	exit(-1);
    }

    // bind prog, unless tracing enabled
    if( ! trace_p ){
	prog = PXS_node_bind(&env, prog);
    }

    
    input = malloc( depth * sizeof(struct PXS_ppm_input) );

    // open files
    if( argc == 1 ){
        input[0].f = stdin;
    }else{
	for(i=0; i<depth; i++){
	    input[i].f = pm_openr( argv[i+1] );
	}
    }

    // init input files
    for(i=0; i<depth; i++){
	ppm_readppminit( input[i].f, &input[i].cols, &input[i].rows, &input[i].maxval, &input[i].format );
	input[i].irow = ppm_allocrow( input[i].cols );
    }

    // use size of 1st image
    rows = input[0].rows;
    cols = input[0].cols;
    maxval = input[0].maxval;

    env.xdim  = cols;
    env.ydim  = rows;
    env.zdim  = depth;
    

    // init output
    ppm_writeppminit( stdout, cols, rows, input[0].maxval, 0 );
    orow = ppm_allocrow( cols );

    
    for ( row=0; row<rows; ++row ){
        env.ypos = row;

	for(i=0; i<depth; i++){
	    if( row < input[i].rows ){
		ppm_readppmrow( input[i].f, input[i].irow, input[i].cols, input[i].maxval, input[i].format );
	    }
	}
	
        for( col=0; col<cols; col++ ){
	    float r, g, b, a;

	    env.xpos = col;
	    a = 0;  // NYI

	    for(i=0; i<depth; i++){
		if( row < input[i].rows && col < input[i].cols ){
		    r = PPM_GETR(input[i].irow[col]);
		    g = PPM_GETG(input[i].irow[col]);
		    b = PPM_GETB(input[i].irow[col]);
		}else{
		    r = g = b = 0;
		}
		if( input[i].maxval != 255 ){
		    r = r * 255 / input[i].maxval;
		    g = g * 255 / input[i].maxval;
		    b = b * 255 / input[i].maxval;
		}

		if( trace_p && !i ){
		    fprintf(stderr, "(%d, %d) = <%02x %02x %02x>\n", col, row, (int)r, (int)g, (int)b);
		}

		if( env.r->values[i] ) PXS_node_free( env.r->values[i] );
		if( env.g->values[i] ) PXS_node_free( env.g->values[i] );
		if( env.b->values[i] ) PXS_node_free( env.b->values[i] );
		if( env.a->values[i] ) PXS_node_free( env.a->values[i] );
		
		env.r->values[i] = PXS_node_float_new(r);
		env.g->values[i] = PXS_node_float_new(g);
		env.b->values[i] = PXS_node_float_new(b);
		env.a->values[i] = PXS_node_float_new(a);
	    }

	    // run program
	    if( prog ) PXS_exec_nodes(&env, prog);
	    PXS_imgf_results(&env, &r, &g, &b, &a);

	    if( trace_p ){
		fprintf(stderr, "    result:\t<%02x %02x %02x>\n",
			(int)r, (int)g, (int)b);
	    }

	    if( maxval != 255 ){
		r = r * maxval / 255;
		g = g * maxval / 255;
		b = b * maxval / 255;
	    }
	    
            if( r < 0 ) r = 0;
            if( g < 0 ) g = 0;
            if( b < 0 ) b = 0;
            if( r > maxval ) r = maxval;
            if( g > maxval ) g = maxval;
            if( b > maxval ) b = maxval;

            PPM_ASSIGN( orow[col], r, g, b);

	    // empty stack
	    PXS_func_clear(&env);
        }
        
        ppm_writeppmrow( stdout, orow, cols, maxval, 0 );
    }

    PXS_env_done(&env);

    // close inputs
    
    pm_close( stdout );
    
}
