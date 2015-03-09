#include <limits>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#define MAX_FILE 50
#define MAX_STRING 50

typedef float real;

int main(int argc, char* argv[]){
  unsigned long long next_random = 10;
  for(int i=0; i<100; i++){
	  next_random = next_random * (unsigned long long)25214903917 + 11;
    cout<<(((next_random & 0xFFFF) / (real)65536) - 0.5)/100<<"\t"<<next_random<<"\n";

  }
	
}

