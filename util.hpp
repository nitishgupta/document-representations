#pragma once
typedef float real;

int add_num(int a, int b);

real sigmoid(real s);

real* WeightMat_VecProd(real *word_e, real *doc_e, real * nn_weight,  int context_position, int wd_id, int embed_size);

real* MatVec(real *matrix, real *vector, int embed_size);

real* vecvecT(real *word_e, real *doc_e, int w1, int e_id1, int w2, int e_id2, int embed_size);

real *addVec(real *v1, real *v2, int embed_size);

real dotProd(real *v1, real *v2, int embed_size);

real *weighted_addVec(real *v1, real *v2, real weight, int embed_size);

void addToVec(real *v1, real *v2, real weight, int embed_size);

void MatVec(real *matrix, real *vector, real *r, int embed_size);

void vecvecT_addToMat(real* v1, real *v2, real *mat, real weight, int embed_size);

void scalarVec(real *vec, real scalar, real *r, int embed_size);

void updateVec(real *v1, real *grad, real grad_weight, real learning_rate, real reg_con, int embed_size);
