/*
  Copyright (c) 2007 by Jeff Weisberg
  Author: Jeff Weisberg <jaw @ tcp4me.com>
  Created: 2007-Aug-12 17:31 (EDT)
  Function: 
 
  $Id: diag.c,v 1.1 2007/09/03 16:52:41 jaw Exp $

*/

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

void
die(const char *fmt, ...){
    char buf[1024];
    int l;
    va_list ap;
    
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);

    exit(-1);
}
