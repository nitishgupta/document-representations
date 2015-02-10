#include <limits>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#define MAX_FILE 50

int main(int argc, char* argv[]){
	/*int imin = std::numeric_limits<unsigned int>::min(); // minimum value
	int imax = std::numeric_limits<unsigned int>::max();*/
	FILE *fin;
	fin = fopen("data/docs.txt", "r");

	int numFiles=5, i=0;
	string line;
  	ifstream myfile ("data/docs.txt");

  	char **files = (char **)calloc(numFiles, sizeof(char*));

  	if (myfile.is_open()) {
  		getline(myfile, line);
  		int numFiles = stoi(line, NULL, 10);
  		cout<<"Num Files : "<<numFiles<<"\n";
    	for(int i=0; i<numFiles; i++){
    		getline(myfile, line);
    		files[i] = (char *)malloc(line.size()+1);
    		std::copy(line.begin(), line.end(), files[i]);
    		files[i][line.size()] = '\0';
    		cout<<files[i]<<"\n";
    		fflush(stdout);
    	}
    	myfile.close();
  	}

  	

  	
	
}

