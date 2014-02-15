/*
  Copyright (c) 2007 by Jeff Weisberg
  Author: Jeff Weisberg <jaw @ tcp4me.com>
  Created: 2007-Aug-13 23:25 (EDT)
  Function: 
 
  $Id: imgf.c,v 1.4 2008/03/26 01:43:12 jaw Exp $

*/

#include <stdio.h>
#include <math.h>
#include "pixelscript.h"

void
PXS_imgf_xdim(PXS_env *env){
    PXS_env_push(env, PXS_node_int_new(env->xdim));
}

void
PXS_imgf_ydim(PXS_env *env){
    PXS_env_push(env, PXS_node_int_new(env->ydim));
}
void
PXS_imgf_zdim(PXS_env *env){
    PXS_env_push(env, PXS_node_int_new(env->zdim));
}

void
PXS_imgf_x(PXS_env *env){
    PXS_env_push(env, PXS_node_int_new(env->xpos));
}

void
PXS_imgf_y(PXS_env *env){
    PXS_env_push(env, PXS_node_int_new(env->ypos));
}

void
PXS_imgf_r(PXS_env *env){
    PXS_node_retain(env->r->values[0]);
    PXS_env_push(env, env->r->values[0]);
}

void
PXS_imgf_g(PXS_env *env){
    PXS_node_retain(env->g->values[0]);
    PXS_env_push(env, env->g->values[0]);
}

void
PXS_imgf_b(PXS_env *env){
    PXS_node_retain(env->b->values[0]);
    PXS_env_push(env, env->b->values[0]);
}

void
PXS_imgf_a(PXS_env *env){
    PXS_node_retain(env->a->values[0]);
    PXS_env_push(env, env->a->values[0]);
}

// return array
void
PXS_imgf_R(PXS_env *env){
    PXS_node_retain( env->r );
    PXS_env_push(env, (PXS_node*)env->r);
}
void
PXS_imgf_G(PXS_env *env){
    PXS_node_retain( env->g );
    PXS_env_push(env, (PXS_node*)env->g);
}
void
PXS_imgf_B(PXS_env *env){
    PXS_node_retain( env->b );
    PXS_env_push(env, (PXS_node*)env->b);
}
void
PXS_imgf_A(PXS_env *env){
    PXS_node_retain( env->a );
    PXS_env_push(env, (PXS_node*)env->a);
}

void
PXS_imgf_results(PXS_env *env, float *r, float *g, float *b, float *a){
    PXS_list_elem *le = env->stack.tail;
    PXS_node *n;
    
    if( le ){
	n = le->node;
	*r =  *g = *b = PXS_number_to_double(n);
	le = le->prev;
    }
    if( le ){
	n = le->node;
	*r = *g = PXS_number_to_double(n);
	le = le->prev;
    }
    if( le ){
	n = le->node;
	*r = PXS_number_to_double(n);
	le = le->prev;
    }
    if( le ){
	n = le->node;
	*a = PXS_number_to_double(n);
	le = le->prev;
    }
}

void
PXS_imgf_gamma(PXS_env *env){
    PXS_node *gn = PXS_env_pop(env);
    PXS_node *cn = PXS_env_pop(env);

    float gv = PXS_number_to_double(gn);
    float cv = PXS_number_to_double(cn);

    PXS_node_free(gn);
    PXS_node_free(cn);

    float nv = 255.0 * powf( (cv / 255.0), gv );
    PXS_env_push(env, PXS_node_float_new(nv));
}

void
PXS_imgf_dist(PXS_env *env){
    PXS_node *yn = PXS_env_pop(env);
    PXS_node *xn = PXS_env_pop(env);
    float xd = PXS_number_to_double(xn) - env->xpos;
    float yd = PXS_number_to_double(yn) - env->ypos;
    
    PXS_node_free(xn);
    PXS_node_free(yn);

    PXS_env_push(env, PXS_node_float_new(hypot(xd, yd)));
}


// of current point
// r g b rgbdist dist
void
PXS_imgf_rgbdist(PXS_env *env){
    PXS_node *bn = PXS_env_pop(env);
    PXS_node *gn = PXS_env_pop(env);
    PXS_node *rn = PXS_env_pop(env);

    float ra = PXS_number_to_double(rn);
    float ga = PXS_number_to_double(gn);
    float ba = PXS_number_to_double(bn);
    float rb = PXS_number_to_double( env->r->values[0] );
    float gb = PXS_number_to_double( env->g->values[0] );
    float bb = PXS_number_to_double( env->b->values[0] );

    
    float dist = sqrtf( (ra-rb)*(ra-rb) + (ga-gb)*(ga-gb) + (ba-bb)*(ba-bb) );
    
    PXS_node_free(rn);
    PXS_node_free(gn);
    PXS_node_free(bn);
    
    PXS_env_push(env, PXS_node_float_new(dist));
}

// of current point
// r g b yuvdist ydist uvdist
void
PXS_imgf_yuvdist(PXS_env *env){
    PXS_node *bn = PXS_env_pop(env);
    PXS_node *gn = PXS_env_pop(env);
    PXS_node *rn = PXS_env_pop(env);
    
    float ra = PXS_number_to_double(rn);
    float ga = PXS_number_to_double(gn);
    float ba = PXS_number_to_double(bn);
    float rb = PXS_number_to_double( env->r->values[0] );
    float gb = PXS_number_to_double( env->g->values[0] );
    float bb = PXS_number_to_double( env->b->values[0] );

    float y, u, v;
    
    y = 0.299    * ra + 0.587   * ga + 0.114   * ba;
    u = -0.14713 * ra - 0.28886 * ga + 0.436   * ba;
    v = 0.615    * ra - 0.51499 * ga - 0.10001 * ba;

    float yc, uc, vc;
    yc = 0.299    * rb + 0.587   * gb + 0.114   * bb;
    uc = -0.14713 * rb - 0.28886 * gb + 0.436   * bb;
    vc = 0.615    * rb - 0.51499 * gb - 0.10001 * bb;

    float dist = sqrtf( (u - uc)*(u - uc) + (v - vc)*(v - vc) );
    
    PXS_env_push(env, PXS_node_float_new( fabs(y - yc) ));
    PXS_env_push(env, PXS_node_float_new(dist));
}


// http://en.wikipedia.org/wiki/HSV_color_space
// hsv : 0-1, 0-1, 0-360
void
PXS_imgf_rgb2hsv(PXS_env *env){
    PXS_node *bn = PXS_env_pop(env);
    PXS_node *gn = PXS_env_pop(env);
    PXS_node *rn = PXS_env_pop(env);

    float r = PXS_number_to_double(rn);
    float g = PXS_number_to_double(gn);
    float b = PXS_number_to_double(bn);

    if( r < 0 ) r = 0;
    if( g < 0 ) g = 0;
    if( b < 0 ) b = 0;
    
    r /= 255.0;
    g /= 255.0;
    b /= 255.0;

    float min, max, h, s, v;
    min = (r<g)   ? r : g;
    min = (b<min) ? b : min;

    max = (r>g)   ? r : g;
    max = (b>max) ? b : max;
    
    if( max == min ) h = 0;
    else if( max == r ){
	if( g >= b ){
	    h = 60 * (g - b) / (max - min);
	}else{
	    h = 60 * (g - b) / (max - min) + 360;
	}
    }else if( max == g ){
	h = 60 * (b - r) / (max - min) + 120;
    }else if( max == b ){
	h = 60 * (r - g) / (max - min) + 240;
    }else{
	// QQQ ?
	h = 0;
    }

    if( max == 0 ) s = 0;
    else s = 1 - min/max;

    v = max;

    PXS_node_free(rn);
    PXS_node_free(gn);
    PXS_node_free(bn);
    
    PXS_env_push(env, PXS_node_float_new(h));
    PXS_env_push(env, PXS_node_float_new(s));
    PXS_env_push(env, PXS_node_float_new(v));
    
}

void
PXS_imgf_hsv2rgb(PXS_env *env){
    PXS_node *vn = PXS_env_pop(env);
    PXS_node *sn = PXS_env_pop(env);
    PXS_node *hn = PXS_env_pop(env);

    float h = PXS_number_to_double(hn);
    float s = PXS_number_to_double(sn);
    float v = PXS_number_to_double(vn);

    h = fmodf( (h + 360), 360.0 );
    
    if( h < 0 ) h = 0;
    if( s < 0 ) s = 0;
    if( v < 0 ) v = 0;
    
    int   hi = (int)(h/60) % 6;
    float f  = h/60 - hi;
    float p  = v * (1 - s);
    float q  = v * (1 - f * s);
    float t  = v * (1 - (1 - f) * s);
    float r, g, b;
    
    switch(hi){
      case 0: r = v; g = t; b = p; break;
      case 1: r = q; g = v; b = p; break;
      case 2: r = p; g = v; b = t; break;
      case 3: r = p; g = q; b = v; break;
      case 4: r = t; g = p; b = v; break;
      case 5: r = v; g = p; b = q; break;
    }

    PXS_node_free(hn);
    PXS_node_free(sn);
    PXS_node_free(vn);

    PXS_env_push(env, PXS_node_float_new(r * 255.0));
    PXS_env_push(env, PXS_node_float_new(g * 255.0));
    PXS_env_push(env, PXS_node_float_new(b * 255.0));

}

// http://en.wikipedia.org/wiki/YUV

void
PXS_imgf_rgb2yuv(PXS_env *env){
    PXS_node *bn = PXS_env_pop(env);
    PXS_node *gn = PXS_env_pop(env);
    PXS_node *rn = PXS_env_pop(env);

    float r = PXS_number_to_double(rn);
    float g = PXS_number_to_double(gn);
    float b = PXS_number_to_double(bn);

    float y, u, v;
    
    y = 0.299    * r + 0.587   * g + 0.114   * b;
    u = -0.14713 * r - 0.28886 * g + 0.436   * b;
    v = 0.615    * r - 0.51499 * g - 0.10001 * b;

    PXS_node_free(rn);
    PXS_node_free(gn);
    PXS_node_free(bn);
    
    PXS_env_push(env, PXS_node_float_new(y));
    PXS_env_push(env, PXS_node_float_new(u));
    PXS_env_push(env, PXS_node_float_new(v));
}

void
PXS_imgf_yuv2rgb(PXS_env *env){
    PXS_node *vn = PXS_env_pop(env);
    PXS_node *un = PXS_env_pop(env);
    PXS_node *yn = PXS_env_pop(env);

    float y = PXS_number_to_double(yn);
    float u = PXS_number_to_double(un);
    float v = PXS_number_to_double(vn);

    float r, g, b;
    r = y + 1.13983 * v;
    g = y - 0.39465 * u - 0.58060 * v;
    b = y + 2.03211 * u;

    PXS_node_free(yn);
    PXS_node_free(un);
    PXS_node_free(vn);

    PXS_env_push(env, PXS_node_float_new(r));
    PXS_env_push(env, PXS_node_float_new(g));
    PXS_env_push(env, PXS_node_float_new(b));
}

void
PXS_imgf_rgb2ymp(PXS_env *env){
    PXS_node *bn = PXS_env_pop(env);
    PXS_node *gn = PXS_env_pop(env);
    PXS_node *rn = PXS_env_pop(env);

    float r = PXS_number_to_double(rn);
    float g = PXS_number_to_double(gn);
    float b = PXS_number_to_double(bn);

    float y, u, v, m, p;
    
    y = 0.299    * r + 0.587   * g + 0.114   * b;
    u = -0.14713 * r - 0.28886 * g + 0.436   * b;
    v = 0.615    * r - 0.51499 * g - 0.10001 * b;

    m = hypot( u, v );
    p = atan2( v, u ) * 180 / M_PI;	// convert radians to degrees
    
    PXS_node_free(rn);
    PXS_node_free(gn);
    PXS_node_free(bn);
    
    PXS_env_push(env, PXS_node_float_new(y));
    PXS_env_push(env, PXS_node_float_new(m));
    PXS_env_push(env, PXS_node_float_new(p));
}

void
PXS_imgf_ymp2rgb(PXS_env *env){
    PXS_node *pn = PXS_env_pop(env);
    PXS_node *mn = PXS_env_pop(env);
    PXS_node *yn = PXS_env_pop(env);

    float y = PXS_number_to_double(yn);
    float m = PXS_number_to_double(mn);
    float p = PXS_number_to_double(pn) * M_PI / 180;	// degrees to radians
    float u = m * cos(p);
    float v = m * sin(p);

    float r, g, b;
    r = y + 1.13983 * v;
    g = y - 0.39465 * u - 0.58060 * v;
    b = y + 2.03211 * u;

    PXS_node_free(yn);
    PXS_node_free(mn);
    PXS_node_free(pn);

    PXS_env_push(env, PXS_node_float_new(r));
    PXS_env_push(env, PXS_node_float_new(g));
    PXS_env_push(env, PXS_node_float_new(b));
}

void
PXS_imgf_init(PXS_env *env){

#define I(a,b)    PXS_env_code_def(env, a, b)
    
    I("width",   PXS_imgf_xdim);
    I("height",  PXS_imgf_ydim);
    I("depth",   PXS_imgf_zdim);
    I("x",       PXS_imgf_x);
    I("y",       PXS_imgf_y);
    I("r",       PXS_imgf_r);
    I("g",       PXS_imgf_g);
    I("b",       PXS_imgf_b);
    I("a",       PXS_imgf_a);
    I("R",       PXS_imgf_R);
    I("G",       PXS_imgf_G);
    I("B",       PXS_imgf_B);
    I("A",       PXS_imgf_A);
    I("gamma",   PXS_imgf_gamma);
    I("dist",    PXS_imgf_dist);
    I("rgbdist", PXS_imgf_rgbdist);
    I("yuvdist", PXS_imgf_yuvdist);
    I("rgb2hsv", PXS_imgf_rgb2hsv);
    I("hsv2rgb", PXS_imgf_hsv2rgb);
    I("rgb2yuv", PXS_imgf_rgb2yuv);
    I("yuv2rgb", PXS_imgf_yuv2rgb);
    I("rgb2ymp", PXS_imgf_rgb2ymp);
    I("ymp2rgb", PXS_imgf_ymp2rgb);

#undef I
}
