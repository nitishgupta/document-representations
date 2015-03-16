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
	char file_name[max_size], st[100][max_size];
	char out[10000];
	strcpy(file_name, argv[1]);
	string line;
	ifstream myfile;
	myfile.open(file_name);
	int w, s;
	getline (myfile,line, '\t'); w = stoi(line); 
	getline (myfile,line, '\t'); s = stoi(line); 
	cout<<line<<"\n\n\n";
	if (myfile.is_open()){
		//for(int j = 0; j<w; j++){
		while ( getline (myfile,line, '\t') ){
			//getline (myfile,line, '\t');
			cout<<"word : "<<line<<" : ";
			for(int i =0; i<s; i++){
				getline (myfile,line, '\t');
				cout<<line<<" ";
			}
			// if(line!=" " || line!="\n"){
   //   			cout << line;
   //   			strcat(out, line.c_str());
   //   		}
    	}
    	myfile.close();
  	}

  	cout<<out;

	// char *words = (char *)malloc(1000 * sizeof(char));
	// char word[100];
	// string a = "nitish";
	// for(int i = 0; i<10; i++)
	// 	strcat(words + i*10, "nitish\n");
	// cout<<words+11<<"\n";
	
}

