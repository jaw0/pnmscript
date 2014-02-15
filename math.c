// AUTO GENERATED - DO NOT EDIT

#include <stdlib.h>
#include <math.h>
#include "pixelscript.h"


void
PXS_mathop_cvi(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    int ai = PXS_number_to_int(a);

    PXS_node_free(a);
    PXS_env_push(env, PXS_node_int_new(ai));
}

void
PXS_mathop_cvr(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    double ad = PXS_number_to_double(a);

    PXS_node_free(a);
    PXS_env_push(env, PXS_node_float_new(ad));
}

void
PXS_mathop_rand(PXS_env *env){
    PXS_env_push(env, PXS_node_float_new(drand48()));
}

void
PXS_mathop_add(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    PXS_node *b = PXS_env_pop(env);
    PXS_node *r;

    if( a->type == PXS_NTYPE_INT && b->type == PXS_NTYPE_INT ){
	int ai = PXS_number_to_int(a);
	int bi = PXS_number_to_int(b);
	int ri = bi + ai;
	r = PXS_node_int_new(ri);
    }else{
	double ad = PXS_number_to_double(a);
	double bd = PXS_number_to_double(b);
	double rd = bd + ad;
	r = PXS_node_float_new(rd);
    }
    
    PXS_node_free(a);
    PXS_node_free(b);
    PXS_env_push(env, r);
}

void
PXS_mathop_sub(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    PXS_node *b = PXS_env_pop(env);
    PXS_node *r;

    if( a->type == PXS_NTYPE_INT && b->type == PXS_NTYPE_INT ){
	int ai = PXS_number_to_int(a);
	int bi = PXS_number_to_int(b);
	int ri = bi - ai;
	r = PXS_node_int_new(ri);
    }else{
	double ad = PXS_number_to_double(a);
	double bd = PXS_number_to_double(b);
	double rd = bd - ad;
	r = PXS_node_float_new(rd);
    }
    
    PXS_node_free(a);
    PXS_node_free(b);
    PXS_env_push(env, r);
}

void
PXS_mathop_mul(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    PXS_node *b = PXS_env_pop(env);
    PXS_node *r;

    if( a->type == PXS_NTYPE_INT && b->type == PXS_NTYPE_INT ){
	int ai = PXS_number_to_int(a);
	int bi = PXS_number_to_int(b);
	int ri = bi * ai;
	r = PXS_node_int_new(ri);
    }else{
	double ad = PXS_number_to_double(a);
	double bd = PXS_number_to_double(b);
	double rd = bd * ad;
	r = PXS_node_float_new(rd);
    }
    
    PXS_node_free(a);
    PXS_node_free(b);
    PXS_env_push(env, r);
}

void
PXS_mathop_div(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    PXS_node *b = PXS_env_pop(env);

    double ad = PXS_number_to_double(a);
    double bd = PXS_number_to_double(b);
    double rd = bd / ad;
    
    PXS_node_free(a);
    PXS_node_free(b);
    PXS_env_push(env, PXS_node_float_new(rd));
}

void
PXS_mathop_lt(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    PXS_node *b = PXS_env_pop(env);
    PXS_node *r;

    if( a->type == PXS_NTYPE_INT && b->type == PXS_NTYPE_INT ){
	int ai = PXS_number_to_int(a);
	int bi = PXS_number_to_int(b);
	int ri = (bi < ai) ? 1 : 0;
	r = PXS_node_int_new(ri);
    }else{
	double ad = PXS_number_to_double(a);
	double bd = PXS_number_to_double(b);
	int ri = (bd < ad) ? 1 : 0;
	r = PXS_node_int_new(ri);
    }
    
    PXS_node_free(a);
    PXS_node_free(b);
    PXS_env_push(env, r);
}

void
PXS_mathop_gt(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    PXS_node *b = PXS_env_pop(env);
    PXS_node *r;

    if( a->type == PXS_NTYPE_INT && b->type == PXS_NTYPE_INT ){
	int ai = PXS_number_to_int(a);
	int bi = PXS_number_to_int(b);
	int ri = (bi > ai) ? 1 : 0;
	r = PXS_node_int_new(ri);
    }else{
	double ad = PXS_number_to_double(a);
	double bd = PXS_number_to_double(b);
	int ri = (bd > ad) ? 1 : 0;
	r = PXS_node_int_new(ri);
    }
    
    PXS_node_free(a);
    PXS_node_free(b);
    PXS_env_push(env, r);
}

void
PXS_mathop_le(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    PXS_node *b = PXS_env_pop(env);
    PXS_node *r;

    if( a->type == PXS_NTYPE_INT && b->type == PXS_NTYPE_INT ){
	int ai = PXS_number_to_int(a);
	int bi = PXS_number_to_int(b);
	int ri = (bi <= ai) ? 1 : 0;
	r = PXS_node_int_new(ri);
    }else{
	double ad = PXS_number_to_double(a);
	double bd = PXS_number_to_double(b);
	int ri = (bd <= ad) ? 1 : 0;
	r = PXS_node_int_new(ri);
    }
    
    PXS_node_free(a);
    PXS_node_free(b);
    PXS_env_push(env, r);
}

void
PXS_mathop_ge(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    PXS_node *b = PXS_env_pop(env);
    PXS_node *r;

    if( a->type == PXS_NTYPE_INT && b->type == PXS_NTYPE_INT ){
	int ai = PXS_number_to_int(a);
	int bi = PXS_number_to_int(b);
	int ri = (bi >= ai) ? 1 : 0;
	r = PXS_node_int_new(ri);
    }else{
	double ad = PXS_number_to_double(a);
	double bd = PXS_number_to_double(b);
	int ri = (bd >= ad) ? 1 : 0;
	r = PXS_node_int_new(ri);
    }
    
    PXS_node_free(a);
    PXS_node_free(b);
    PXS_env_push(env, r);
}

void
PXS_mathop_eq(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    PXS_node *b = PXS_env_pop(env);
    PXS_node *r;

    if( a->type == PXS_NTYPE_INT && b->type == PXS_NTYPE_INT ){
	int ai = PXS_number_to_int(a);
	int bi = PXS_number_to_int(b);
	int ri = (bi == ai) ? 1 : 0;
	r = PXS_node_int_new(ri);
    }else{
	double ad = PXS_number_to_double(a);
	double bd = PXS_number_to_double(b);
	int ri = (bd == ad) ? 1 : 0;
	r = PXS_node_int_new(ri);
    }
    
    PXS_node_free(a);
    PXS_node_free(b);
    PXS_env_push(env, r);
}

void
PXS_mathop_ne(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    PXS_node *b = PXS_env_pop(env);
    PXS_node *r;

    if( a->type == PXS_NTYPE_INT && b->type == PXS_NTYPE_INT ){
	int ai = PXS_number_to_int(a);
	int bi = PXS_number_to_int(b);
	int ri = (bi != ai) ? 1 : 0;
	r = PXS_node_int_new(ri);
    }else{
	double ad = PXS_number_to_double(a);
	double bd = PXS_number_to_double(b);
	int ri = (bd != ad) ? 1 : 0;
	r = PXS_node_int_new(ri);
    }
    
    PXS_node_free(a);
    PXS_node_free(b);
    PXS_env_push(env, r);
}

void
PXS_mathop_imod(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    PXS_node *b = PXS_env_pop(env);
    int ai = PXS_number_to_int(a);
    int bi = PXS_number_to_int(b);

    int ri = bi % ai;
    PXS_node_free(a);
    PXS_node_free(b);

    PXS_env_push(env, PXS_node_int_new(ri));
}

void
PXS_mathop_idiv(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    PXS_node *b = PXS_env_pop(env);
    int ai = PXS_number_to_int(a);
    int bi = PXS_number_to_int(b);

    int ri = bi / ai;
    PXS_node_free(a);
    PXS_node_free(b);

    PXS_env_push(env, PXS_node_int_new(ri));
}

void
PXS_mathop_and(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    PXS_node *b = PXS_env_pop(env);
    int ai = PXS_number_to_int(a);
    int bi = PXS_number_to_int(b);

    int ri = bi && ai;
    PXS_node_free(a);
    PXS_node_free(b);

    PXS_env_push(env, PXS_node_int_new(ri));
}

void
PXS_mathop_or(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    PXS_node *b = PXS_env_pop(env);
    int ai = PXS_number_to_int(a);
    int bi = PXS_number_to_int(b);

    int ri = bi || ai;
    PXS_node_free(a);
    PXS_node_free(b);

    PXS_env_push(env, PXS_node_int_new(ri));
}

void
PXS_mathop_bitand(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    PXS_node *b = PXS_env_pop(env);
    int ai = PXS_number_to_int(a);
    int bi = PXS_number_to_int(b);

    int ri = bi & ai;
    PXS_node_free(a);
    PXS_node_free(b);

    PXS_env_push(env, PXS_node_int_new(ri));
}

void
PXS_mathop_bitor(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    PXS_node *b = PXS_env_pop(env);
    int ai = PXS_number_to_int(a);
    int bi = PXS_number_to_int(b);

    int ri = bi | ai;
    PXS_node_free(a);
    PXS_node_free(b);

    PXS_env_push(env, PXS_node_int_new(ri));
}

void
PXS_mathop_bitxor(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    PXS_node *b = PXS_env_pop(env);
    int ai = PXS_number_to_int(a);
    int bi = PXS_number_to_int(b);

    int ri = bi ^ ai;
    PXS_node_free(a);
    PXS_node_free(b);

    PXS_env_push(env, PXS_node_int_new(ri));
}

void
PXS_mathop_shl(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    PXS_node *b = PXS_env_pop(env);
    int ai = PXS_number_to_int(a);
    int bi = PXS_number_to_int(b);

    int ri = bi << ai;
    PXS_node_free(a);
    PXS_node_free(b);

    PXS_env_push(env, PXS_node_int_new(ri));
}

void
PXS_mathop_shr(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    PXS_node *b = PXS_env_pop(env);
    int ai = PXS_number_to_int(a);
    int bi = PXS_number_to_int(b);

    int ri = bi >> ai;
    PXS_node_free(a);
    PXS_node_free(b);

    PXS_env_push(env, PXS_node_int_new(ri));
}

void
PXS_mathop_neg(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    PXS_node *r;

    if( a->type == PXS_NTYPE_INT ){
	int ai = PXS_number_to_int(a);
	int ri = - ai;
	r = PXS_node_int_new(ri);
    }else{
	double ad = PXS_number_to_double(a);
	double rd = - ad;
	r = PXS_node_float_new(rd);
    }
    
    PXS_node_free(a);
    PXS_env_push(env, r);
}

void
PXS_mathop_not(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    int ai = PXS_number_to_int(a);

    int ri = ! ai;
    PXS_node_free(a);

    PXS_env_push(env, PXS_node_int_new(ri));
}

void
PXS_mathop_bitnot(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    int ai = PXS_number_to_int(a);

    int ri = ~ ai;
    PXS_node_free(a);

    PXS_env_push(env, PXS_node_int_new(ri));
}

void
PXS_mathop_sqrt(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    double ad = PXS_number_to_double(a);
    double rd = sqrt(ad);
    
    PXS_node_free(a);
    PXS_env_push(env, PXS_node_float_new(rd));
}

void
PXS_mathop_cbrt(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    double ad = PXS_number_to_double(a);
    double rd = cbrt(ad);
    
    PXS_node_free(a);
    PXS_env_push(env, PXS_node_float_new(rd));
}

void
PXS_mathop_cos(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    double ad = PXS_number_to_double(a);
    double rd = cos(ad);
    
    PXS_node_free(a);
    PXS_env_push(env, PXS_node_float_new(rd));
}

void
PXS_mathop_sin(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    double ad = PXS_number_to_double(a);
    double rd = sin(ad);
    
    PXS_node_free(a);
    PXS_env_push(env, PXS_node_float_new(rd));
}

void
PXS_mathop_tan(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    double ad = PXS_number_to_double(a);
    double rd = tan(ad);
    
    PXS_node_free(a);
    PXS_env_push(env, PXS_node_float_new(rd));
}

void
PXS_mathop_acos(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    double ad = PXS_number_to_double(a);
    double rd = acos(ad);
    
    PXS_node_free(a);
    PXS_env_push(env, PXS_node_float_new(rd));
}

void
PXS_mathop_asin(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    double ad = PXS_number_to_double(a);
    double rd = asin(ad);
    
    PXS_node_free(a);
    PXS_env_push(env, PXS_node_float_new(rd));
}

void
PXS_mathop_atan(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    double ad = PXS_number_to_double(a);
    double rd = atan(ad);
    
    PXS_node_free(a);
    PXS_env_push(env, PXS_node_float_new(rd));
}

void
PXS_mathop_ceil(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    double ad = PXS_number_to_double(a);
    double rd = ceil(ad);
    
    PXS_node_free(a);
    PXS_env_push(env, PXS_node_float_new(rd));
}

void
PXS_mathop_floor(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    double ad = PXS_number_to_double(a);
    double rd = floor(ad);
    
    PXS_node_free(a);
    PXS_env_push(env, PXS_node_float_new(rd));
}

void
PXS_mathop_exp(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    double ad = PXS_number_to_double(a);
    double rd = exp(ad);
    
    PXS_node_free(a);
    PXS_env_push(env, PXS_node_float_new(rd));
}

void
PXS_mathop_log(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    double ad = PXS_number_to_double(a);
    double rd = log(ad);
    
    PXS_node_free(a);
    PXS_env_push(env, PXS_node_float_new(rd));
}

void
PXS_mathop_cosh(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    double ad = PXS_number_to_double(a);
    double rd = cosh(ad);
    
    PXS_node_free(a);
    PXS_env_push(env, PXS_node_float_new(rd));
}

void
PXS_mathop_sinh(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    double ad = PXS_number_to_double(a);
    double rd = sinh(ad);
    
    PXS_node_free(a);
    PXS_env_push(env, PXS_node_float_new(rd));
}

void
PXS_mathop_tanh(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    double ad = PXS_number_to_double(a);
    double rd = tanh(ad);
    
    PXS_node_free(a);
    PXS_env_push(env, PXS_node_float_new(rd));
}

void
PXS_mathop_acosh(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    double ad = PXS_number_to_double(a);
    double rd = acosh(ad);
    
    PXS_node_free(a);
    PXS_env_push(env, PXS_node_float_new(rd));
}

void
PXS_mathop_asinh(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    double ad = PXS_number_to_double(a);
    double rd = asinh(ad);
    
    PXS_node_free(a);
    PXS_env_push(env, PXS_node_float_new(rd));
}

void
PXS_mathop_atanh(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    double ad = PXS_number_to_double(a);
    double rd = atanh(ad);
    
    PXS_node_free(a);
    PXS_env_push(env, PXS_node_float_new(rd));
}

void
PXS_mathop_rint(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    double ad = PXS_number_to_double(a);
    double rd = rint(ad);
    
    PXS_node_free(a);
    PXS_env_push(env, PXS_node_float_new(rd));
}

void
PXS_mathop_fabs(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    double ad = PXS_number_to_double(a);
    double rd = fabs(ad);
    
    PXS_node_free(a);
    PXS_env_push(env, PXS_node_float_new(rd));
}

void
PXS_mathop_pow(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    PXS_node *b = PXS_env_pop(env);

    double ad = PXS_number_to_double(a);
    double bd = PXS_number_to_double(b);
    double rd = pow(bd, ad);
    if( rd == 0.0 ) rd = pow(bd,ad);
    
    PXS_node_free(a);
    PXS_node_free(b);
    PXS_env_push(env, PXS_node_float_new(rd));
}

void
PXS_mathop_atan2(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    PXS_node *b = PXS_env_pop(env);

    double ad = PXS_number_to_double(a);
    double bd = PXS_number_to_double(b);
    double rd = atan2(bd, ad);
    if( rd == 0.0 ) rd = atan2(bd,ad);
    
    PXS_node_free(a);
    PXS_node_free(b);
    PXS_env_push(env, PXS_node_float_new(rd));
}

void
PXS_mathop_hypot(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    PXS_node *b = PXS_env_pop(env);

    double ad = PXS_number_to_double(a);
    double bd = PXS_number_to_double(b);
    double rd = hypot(bd, ad);
    if( rd == 0.0 ) rd = hypot(bd,ad);
    
    PXS_node_free(a);
    PXS_node_free(b);
    PXS_env_push(env, PXS_node_float_new(rd));
}

void
PXS_math_init(PXS_env *env){
    PXS_env_code_def(env, "cvi", PXS_mathop_cvi);
    PXS_env_code_def(env, "cvr", PXS_mathop_cvr);
    PXS_env_code_def(env, "rand", PXS_mathop_rand);
    PXS_env_code_def(env, "add", PXS_mathop_add);
    PXS_env_code_def(env, "sub", PXS_mathop_sub);
    PXS_env_code_def(env, "mul", PXS_mathop_mul);
    PXS_env_code_def(env, "div", PXS_mathop_div);
    PXS_env_code_def(env, "lt", PXS_mathop_lt);
    PXS_env_code_def(env, "gt", PXS_mathop_gt);
    PXS_env_code_def(env, "le", PXS_mathop_le);
    PXS_env_code_def(env, "ge", PXS_mathop_ge);
    PXS_env_code_def(env, "eq", PXS_mathop_eq);
    PXS_env_code_def(env, "ne", PXS_mathop_ne);
    PXS_env_code_def(env, "imod", PXS_mathop_imod);
    PXS_env_code_def(env, "idiv", PXS_mathop_idiv);
    PXS_env_code_def(env, "and", PXS_mathop_and);
    PXS_env_code_def(env, "or", PXS_mathop_or);
    PXS_env_code_def(env, "bitand", PXS_mathop_bitand);
    PXS_env_code_def(env, "bitor", PXS_mathop_bitor);
    PXS_env_code_def(env, "bitxor", PXS_mathop_bitxor);
    PXS_env_code_def(env, "shl", PXS_mathop_shl);
    PXS_env_code_def(env, "shr", PXS_mathop_shr);
    PXS_env_code_def(env, "neg", PXS_mathop_neg);
    PXS_env_code_def(env, "not", PXS_mathop_not);
    PXS_env_code_def(env, "bitnot", PXS_mathop_bitnot);
    PXS_env_code_def(env, "sqrt", PXS_mathop_sqrt);
    PXS_env_code_def(env, "cbrt", PXS_mathop_cbrt);
    PXS_env_code_def(env, "cos", PXS_mathop_cos);
    PXS_env_code_def(env, "sin", PXS_mathop_sin);
    PXS_env_code_def(env, "tan", PXS_mathop_tan);
    PXS_env_code_def(env, "acos", PXS_mathop_acos);
    PXS_env_code_def(env, "asin", PXS_mathop_asin);
    PXS_env_code_def(env, "atan", PXS_mathop_atan);
    PXS_env_code_def(env, "ceil", PXS_mathop_ceil);
    PXS_env_code_def(env, "floor", PXS_mathop_floor);
    PXS_env_code_def(env, "exp", PXS_mathop_exp);
    PXS_env_code_def(env, "log", PXS_mathop_log);
    PXS_env_code_def(env, "cosh", PXS_mathop_cosh);
    PXS_env_code_def(env, "sinh", PXS_mathop_sinh);
    PXS_env_code_def(env, "tanh", PXS_mathop_tanh);
    PXS_env_code_def(env, "acosh", PXS_mathop_acosh);
    PXS_env_code_def(env, "asinh", PXS_mathop_asinh);
    PXS_env_code_def(env, "atanh", PXS_mathop_atanh);
    PXS_env_code_def(env, "rint", PXS_mathop_rint);
    PXS_env_code_def(env, "fabs", PXS_mathop_fabs);
    PXS_env_code_def(env, "pow", PXS_mathop_pow);
    PXS_env_code_def(env, "atan2", PXS_mathop_atan2);
    PXS_env_code_def(env, "hypot", PXS_mathop_hypot);
}

