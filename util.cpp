#include <stdio.h>
#include <iostream>
#include <math.h>
#include "util.hpp"

typedef float real;
using namespace std;

int add_num(int a, int b){
	return a + b;
}

real sigmoid(real s){
	return 1/(1 + exp(-s));
}

real* WeightMat_VecProd(real *word_e, real *doc_e, real * nn_weight,  int context_position, int wd_id, int embed_size){
	real *r = (real *) calloc(embed_size, sizeof(real));
	if(context_position == 0)	// Weight Matrix * Document Vector
		for(int i=0; i<embed_size; i++)
			for(int j=0; j<embed_size; j++){
				r[i] += nn_weight[i*embed_size + j] * doc_e[wd_id*embed_size + j];
	}
	else{
		for(int i=0; i<embed_size; i++)
			for(int j=0; j<embed_size; j++)
				r[i] += nn_weight[embed_size*embed_size*context_position + i*embed_size + j] * word_e[wd_id*embed_size + j];
	}
	return r;
}

real* MatVec(real *matrix, real *vector, int embed_size){
	real *r = (real *) calloc(embed_size, sizeof(real));
	for(int i=0; i<embed_size; i++){
		for(int j=0; j<embed_size; j++){
			r[i] += matrix[i*embed_size + j] * vector[j];
		}
	}

	return r;
}


void MatVec(real *matrix, real *vector, real *r, int embed_size){
	for(int i=0; i<embed_size; i++){
		r[i] = 0;	
		for(int j=0; j<embed_size; j++){
			r[i] += matrix[i*embed_size + j] * vector[j];
		}
	}
}

void AddMatVecToVec(real *matrix, real *vector, real *r, int embed_size){
	for(int i=0; i<embed_size; i++){
		for(int j=0; j<embed_size; j++){
			r[i] += matrix[i*embed_size + j] * vector[j];
		}
	}
}

void scalarVec(real *vec, real scalar, real *r, int embed_size){
	for(int i=0; i<embed_size; i++){
		r[i] = scalar*vec[i];
	}
}

real* vecvecT(real *word_e, real *doc_e, int w1, int e_id1, int w2, int e_id2, int embed_size){
	real *r = (real *) calloc(embed_size*embed_size, sizeof(real));
	real *v1, *v2;
	if(w1 == 0)	v1 = doc_e + (e_id1*embed_size);
	else		v1 = word_e + (e_id1*embed_size);

	if(w2 == 0)	v2 = doc_e + (e_id2*embed_size);
	else		v2 = word_e + (e_id2*embed_size);

	for(int i=0; i<embed_size; i++)
		for(int j=0; j<embed_size; j++)
			r[i*embed_size + j] = v1[i]*v2[j];

	return r;	
}

void updateVec(real *v1, real *grad, real grad_weight, real learning_rate, real reg_con, int embed_size){
	for(int i=0; i<embed_size; i++){
		v1[i] += learning_rate * (grad[i]*grad_weight - reg_con*v1[i]);
	}
}


void addToVec(real *v1, real *v2, real weight, int embed_size){
	for(int i=0; i<embed_size; i++){
		v1[i] += weight * v2[i];
	}
}

real *addVec(real *v1, real *v2, int embed_size){
	real *r = (real *) calloc(embed_size, sizeof(real));
	for(int i=0; i<embed_size; i++)
		r[i] = v1[i] + v2[i];

	return r;
}

// r = weight*v1 + *v2
// void weighted_addVec(real *v1, real weight, real *v2, real *r, int embed_size){
// 	for(int i=0; i<embed_size; i++){
// 		r[i] = v2[i] + weight*v1[i]
// 	}
// }

// r = v1 + weight*v2
real *weighted_addVec(real *v1, real *v2, real weight, int embed_size){
	real *r = (real *) calloc(embed_size, sizeof(real));
	for(int i=0; i<embed_size; i++)
		r[i] = v1[i] + weight*v2[i];

	return r;
}

void vecvecT_updateMat(real* v1, real *v2, real *mat, real grad_weight, real learning_rate, real reg_con, int embed_size){
	for(int i=0; i<embed_size; i++)
		for(int j=0; j<embed_size; j++)
			mat[embed_size*i + j] += learning_rate*(grad_weight*v1[i]*v2[j] - reg_con*mat[embed_size*i + j]);
}

real dotProd(real *v1, real *v2, int embed_size){
	real r = 0;
	for(int i=0; i<embed_size; i++)
		r += v1[i]*v2[i];

	return r;
}

