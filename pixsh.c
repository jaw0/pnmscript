/*
  Copyright (c) 2007 by Jeff Weisberg
  Author: Jeff Weisberg <jaw @ tcp4me.com>
  Created: 2007-Aug-12 14:23 (EDT)
  Function: 
 
  $Id: pixsh.c,v 1.1 2007/09/03 16:52:44 jaw Exp $

*/

#include <stdio.h>
#include "pixelscript.h"

PXS_env env;

main(){
    struct PXS_node *prog;

    PXS_env_init(&env);
    
    prog = PXS_compile_string(&env, "executive");
    PXS_exec_nodes(&env, prog);
    
}
