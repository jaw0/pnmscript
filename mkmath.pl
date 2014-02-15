#!/usr/local/bin/perl
# -*- perl -*-

# Copyright (c) 2007 by Jeff Weisberg
# Author: Jeff Weisberg <jaw @ tcp4me.com>
# Created: 2007-Aug-13 21:52 (EDT)
# Function: create math functions
#
# $Id: mkmath.pl,v 1.3 2008/03/25 22:34:13 jaw Exp $

use strict;

my @init = qw(cvi cvr rand);

my @op =
(
 # take 2 integers or floats, returns same
 { op => '+',  name => 'add', type => 'n2n' },
 { op => '-',  name => 'sub', type => 'n2n' },
 { op => '*',  name => 'mul', type => 'n2n' },
 { op => '/',  name => 'div', type => 'd2d' },	# always as floats, see also idiv
 # compare 2 integers or floats, return integer: 0 or 1
 { op => '<',  name => 'lt',  type => 'n2b' },
 { op => '>',  name => 'gt',  type => 'n2b' },
 { op => '<=', name => 'le',  type => 'n2b' },
 { op => '>=', name => 'ge',  type => 'n2b' },
 { op => '==', name => 'eq',  type => 'n2b' },
 { op => '!=', name => 'ne',  type => 'n2b' },
 # convert 2 args to integers, return integer
 { op => '%',  name => 'imod',   type => 'i2i' },
 { op => '/',  name => 'idiv',   type => 'i2i' },
 { op => '&&', name => 'and',    type => 'i2i' },
 { op => '||', name => 'or',     type => 'i2i' },
 { op => '&',  name => 'bitand', type => 'i2i' },
 { op => '|',  name => 'bitor',  type => 'i2i' },
 { op => '^',  name => 'bitxor', type => 'i2i' },
 { op => '<<', name => 'shl',    type => 'i2i' },
 { op => '>>', name => 'shr',    type => 'i2i' },
 # unary ops
 { op => '-',  name => 'neg',     type => 'n1n' },
 { op => '!',  name => 'not',     type => 'i1i' },
 { op => '~',  name => 'bitnot',  type => 'i1i' },
 
 );

# functions from libm
my @d1d = qw(sqrt cbrt cos sin tan acos asin atan ceil floor exp log cosh sinh tanh acosh asinh atanh rint fabs);
my @d2d = qw(pow atan2 hypot);


while(<DATA>){
    print;
}

my %ft = ( i1i => \&i1i_make,
	   i2i => \&i2i_make,
	   n1n => \&n1n_make,
	   n2b => \&n2b_make,
	   n2n => \&n2n_make,
	   d2d => \&d2d_make,
	   d1d => \&d1d_make,
	   );
	   
for my $x (@op){
    push @init, $x->{name};

    my $f = $ft{ $x->{type} };
    die "unknown type $x->{type}\n" unless $f;
    $f->( $x );
}

for my $f (@d1d){
    d1df_make( { name => $f, op => $f } );
}

for my $f (@d2d){
    d2df_make( { name => $f, op => $f } );
}

push @init, @d1d, @d2d;

# init code
print "void\nPXS_math_init(PXS_env *env){\n";
for my $nm (@init){
    print "    PXS_env_code_def(env, \"$nm\", PXS_mathop_$nm);\n";
}
print "}\n\n";

################################################################

sub d1df_make {
    my $x = shift;

    my $op = $x->{op};
    my $nm = $x->{name};

    print <<EOSUB;
void
PXS_mathop_$nm(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    double ad = PXS_number_to_double(a);
    double rd = $op(ad);
    
    PXS_node_free(a);
    PXS_env_push(env, PXS_node_float_new(rd));
}

EOSUB
	;
    
}
sub d2df_make {
    my $x = shift;

    my $op = $x->{op};
    my $nm = $x->{name};

    # something odd is going on...
    my $again;
    $again = "if( rd == 0.0 ) rd = $op(bd,ad);";

    print <<EOSUB;
void
PXS_mathop_$nm(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    PXS_node *b = PXS_env_pop(env);

    double ad = PXS_number_to_double(a);
    double bd = PXS_number_to_double(b);
    double rd = $op(bd, ad);
    $again
    
    PXS_node_free(a);
    PXS_node_free(b);
    PXS_env_push(env, PXS_node_float_new(rd));
}

EOSUB
    ;
    
}

sub d2d_make {
    my $x = shift;

    my $op = $x->{op};
    my $nm = $x->{name};

    print <<EOSUB;
void
PXS_mathop_$nm(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    PXS_node *b = PXS_env_pop(env);

    double ad = PXS_number_to_double(a);
    double bd = PXS_number_to_double(b);
    double rd = bd $op ad;
    
    PXS_node_free(a);
    PXS_node_free(b);
    PXS_env_push(env, PXS_node_float_new(rd));
}

EOSUB
    ;
    
}


sub i1i_make {
    my $x = shift;

    my $op = $x->{op};
    my $nm = $x->{name};

    print <<EOSUB;
void
PXS_mathop_$nm(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    int ai = PXS_number_to_int(a);

    int ri = $op ai;
    PXS_node_free(a);

    PXS_env_push(env, PXS_node_int_new(ri));
}

EOSUB
    ;
}

sub i2i_make {
    my $x = shift;

    my $op = $x->{op};
    my $nm = $x->{name};

    print <<EOSUB;
void
PXS_mathop_$nm(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    PXS_node *b = PXS_env_pop(env);
    int ai = PXS_number_to_int(a);
    int bi = PXS_number_to_int(b);

    int ri = bi $op ai;
    PXS_node_free(a);
    PXS_node_free(b);

    PXS_env_push(env, PXS_node_int_new(ri));
}

EOSUB
    ;
}

sub n1n_make {
    my $x = shift;

    my $op = $x->{op};
    my $nm = $x->{name};
    
    print <<EOSUB;
void
PXS_mathop_$nm(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    PXS_node *r;

    if( a->type == PXS_NTYPE_INT ){
	int ai = PXS_number_to_int(a);
	int ri = $op ai;
	r = PXS_node_int_new(ri);
    }else{
	double ad = PXS_number_to_double(a);
	double rd = $op ad;
	r = PXS_node_float_new(rd);
    }
    
    PXS_node_free(a);
    PXS_env_push(env, r);
}

EOSUB
    ;
    
}

sub n2n_make {
    my $x = shift;

    my $op = $x->{op};
    my $nm = $x->{name};
    
    print <<EOSUB;
void
PXS_mathop_$nm(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    PXS_node *b = PXS_env_pop(env);
    PXS_node *r;

    if( a->type == PXS_NTYPE_INT && b->type == PXS_NTYPE_INT ){
	int ai = PXS_number_to_int(a);
	int bi = PXS_number_to_int(b);
	int ri = bi $op ai;
	r = PXS_node_int_new(ri);
    }else{
	double ad = PXS_number_to_double(a);
	double bd = PXS_number_to_double(b);
	double rd = bd $op ad;
	r = PXS_node_float_new(rd);
    }
    
    PXS_node_free(a);
    PXS_node_free(b);
    PXS_env_push(env, r);
}

EOSUB
    ;
    
}
sub n2b_make {
    my $x = shift;

    my $op = $x->{op};
    my $nm = $x->{name};
    
    print <<EOSUB;
void
PXS_mathop_$nm(PXS_env *env){
    PXS_node *a = PXS_env_pop(env);
    PXS_node *b = PXS_env_pop(env);
    PXS_node *r;

    if( a->type == PXS_NTYPE_INT && b->type == PXS_NTYPE_INT ){
	int ai = PXS_number_to_int(a);
	int bi = PXS_number_to_int(b);
	int ri = (bi $op ai) ? 1 : 0;
	r = PXS_node_int_new(ri);
    }else{
	double ad = PXS_number_to_double(a);
	double bd = PXS_number_to_double(b);
	int ri = (bd $op ad) ? 1 : 0;
	r = PXS_node_int_new(ri);
    }
    
    PXS_node_free(a);
    PXS_node_free(b);
    PXS_env_push(env, r);
}

EOSUB
    ;
    
}

__END__
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

