/*
  Copyright (c) 2007 by Jeff Weisberg
  Author: Jeff Weisberg <jaw @ tcp4me.com>
  Created: 2007-Aug-14 20:17 (EDT)
  Function: memory allocation
 
  $Id: alloc.c,v 1.1 2007/09/03 16:52:41 jaw Exp $

*/

#include "pixelscript.h"

/* according to the stats collector, most of our
   allocs are 12, 16, or 20 bytes

   we maintain our own set of memory pools to quickly
   allocate 20 byte chunks
*/

#define ALLOCSIZE	20
#define POOLSIZE	819
// NB: 819 * 20 just under 16k

// NB: both of these structs must be smaller than ALLOCSIZE
// all free blocks are on a linked-list
struct freelist {
    struct freelist *next;
};
// all pools of blocks are on a linked-list
struct poolhdr {
    struct poolhdr *next;
    void *last;		// last byte in pool
};

static struct poolhdr  *poollist = 0;
static struct freelist *freelist = 0;

static void
embiggen_pool(void){
    int i;
    void *p = (void*)malloc( ALLOCSIZE * POOLSIZE );
    if(!p) die("out of memory!");

    // put pool on poollist
    struct poolhdr *ph = (struct poolhdr *)p;
    ph->next = poollist;
    ph->last = (char*)p + ALLOCSIZE * POOLSIZE - 1;
    poollist = ph;
    
    // put memory on freelist
    // skip poolheader
    for(i=POOLSIZE-1; i>=1; i--){
	struct freelist *pf = (struct freelist*)((char*)p + (i * ALLOCSIZE));
	pf->next = freelist;
	freelist = pf;
    }
}

void
PXS_free(void *p){

    // from a pool?
    struct poolhdr *pl = poollist;

    while(pl){
	if( p > (void*)pl && p < pl->last ){
	    // found pool. put on freelist
	    struct freelist *pf = (struct freelist*)p;
	    pf->next = freelist;
	    freelist = pf;
	    return;
	}
	pl = pl->next;
    }

    // otherwise it was from the system
    free(p);
}

void *
PXS_malloc(int size){
    void *p;

    if( size <= ALLOCSIZE ){
	// small chunks - from our pool
	if(!freelist){
	    embiggen_pool();
	}

	p = freelist;
	freelist = freelist->next;
	
    }else{
	// big chunk - from system
	p = (void*)malloc(size);
    }
    if(!p){
	die("out of memory!");
    }

    return p;
}

void *
PXS_salloc(int size){
    void *p = (void*)malloc(size);
    if(!p) die("out of memory!");
    return p;
}


// alloc and zero
void *
PXS_zalloc(int size){
    void *p = PXS_malloc(size);
    bzero(p, size);
    return p;
}

