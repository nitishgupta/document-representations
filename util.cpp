#include <stdio.h>
#include <iostream>

#include "util.hpp"

typedef float real;

int add_num(int a, int b){
	return a + b;
}

real* weightMatrix_vector(real* word_e, real *doc_e, real * nn_weight,  int context_position, int wd_id, int embed_size){
	real *r = (real *) calloc(embed_size, sizeof(real));
	if(context_position == 0){	// Weight Matrix * Document Vector
		for(int i=0; i<embed_size; i++){
			for(int j=0; j<embed_size; j++){
				r[i] += nn_weight[i*embed_size + j] * doc_e[wd_id*embed_size + j];
			}
		}	
	}

	else{

		for(int i=0; i<embed_size; i++){
			for(int j = 0; j < embed_size; j++){
				std::cout<<nn_weight[embed_size*embed_size*context_position + i*embed_size + j] << "\t";
			}
			std::cout<<"\n";
		}

		for(int i=0; i<embed_size; i++)
			std::cout<<word_e[wd_id*embed_size + i]<<" ";
		std::cout<"\n";


		std::cout<<"\nWORD\n";
		for(int i=0; i<embed_size; i++){
			for(int j=0; j<embed_size; j++){
				r[i] += nn_weight[embed_size*embed_size*context_position + i*embed_size + j] * word_e[wd_id*embed_size + j];
			}
		}	
	}

	return r;
	
}