/*
  Copyright (c) 2007 by Jeff Weisberg
  Author: Jeff Weisberg <jaw @ tcp4me.com>
  Created: 2007-Aug-12 14:23 (EDT)
  Function: 
 
  $Id: test.c,v 1.1 2007/09/03 16:52:44 jaw Exp $

*/

#include <stdio.h>
#include "pixelscript.h"

PXS_env env;

main(){
    struct PXS_node *prog;

    PXS_env_init(&env);
    env.xdim = env.ydim = 1023;
    
    prog = PXS_compile_string(&env, "/foo 123 def foo 2 add == "
	" { /foo foo 1 add def foo 130 gt { break } if foo == } loop "
        " 0 1 5 { == } for "
	" INIT { /init == } "
	" 1 2 3 4 5 6 7 8 9 3 -1 roll pstack "
	" 'done' = executive");
    
    PXS_exec_nodes(&env, prog);

    
}
