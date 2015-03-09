#pragma once
typedef float real;

int add_num(int a, int b);

real* weightMatrix_vector(real* word_e, real *doc_e, real * nn_weight,  int context_position, int wd_id, int embed_size);
