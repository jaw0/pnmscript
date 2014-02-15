/*
  Copyright (c) 2007 by Jeff Weisberg
  Author: Jeff Weisberg <jaw @ tcp4me.com>
  Created: 2007-Aug-13 21:51 (EDT)
  Function: miscellaneous...
 
  $Id: misc.c,v 1.2 2008/03/26 01:43:14 jaw Exp $

*/

#include "pixelscript.h"

double
PXS_number_to_double(PXS_node *node){
    PXS_node_int   *in;
    PXS_node_float *fn;
    
    switch(node->type){
      case PXS_NTYPE_INT:
	    in = (PXS_node_int*)node;
	    return (double)in->value;
	    
      case PXS_NTYPE_FLOAT:
	    fn = (PXS_node_float*)node;
	    return fn->value;
	    
      default:
	    PXS_error(0, node, "number_to_double", "cannot convert type to number");
    }
}

int
PXS_number_to_int(PXS_node *node){
    PXS_node_int   *in;
    PXS_node_float *fn;
    
    switch(node->type){
      case PXS_NTYPE_INT:
	    in = (PXS_node_int*)node;
	    return in->value;
	    
      case PXS_NTYPE_FLOAT:
	    fn = (PXS_node_float*)node;
	    return (int)fn->value;
	    
      default:
	    PXS_error(0, node, "number_to_int", "cannot convert type to number");
    }
}
