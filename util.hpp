#pragma once
typedef float real;

int add_num(int a, int b);

real sigmoid(real s);

real* MatVec(real* word_e, real *doc_e, real * nn_weight,  int context_position, int wd_id, int embed_size);

real* vecvecT(real *word_e, real *doc_e, int w1, int e_id1, int w2, int e_id2, int embed_size);

real *addVec(real *v1, real *v2, int embed_size);

real dotProd(real *v1, real *v2, int embed_size);

real *weighted_addVec(real *v1, real *v2, real weight, int embed_size);
