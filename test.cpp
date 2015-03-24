#include <limits>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>
using namespace std;

#define MAX_FILE 50
#define MAX_STRING 50
#define max_size 100
typedef float real;


int main(int argc, char* argv[]){
	unsigned long long next_random = 1;		

	next_random = next_random * (unsigned long long)25214903917 + 11;
	for(int i = 0; i < 20; i++)
		cout<< (((next_random & 0xFFFF) / (real)65536) - 0.5) / (100)<<"\n";
	return 0;
}

