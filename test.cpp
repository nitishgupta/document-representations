#include <limits>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#define MAX_FILE 50
#define MAX_STRING 50

void ReadWord(char *word, FILE *fin) {
  int a = 0, ch;
  while (!feof(fin)) {
    ch = fgetc(fin);
    if ((ch == 13) || (ch == ' ') || (ch == '\t') || (ch == '\n')) {
      if (a==0)
      		continue;
      else 
        break; 
    }
    word[a] = ch;
    a++;
    if (a >= MAX_STRING - 1) a--;   // Truncate too long words
  }
  word[a] = '\0';
}

int main(int argc, char* argv[]){
	/*int imin = std::numeric_limits<unsigned int>::min(); // minimum value
	int imax = std::numeric_limits<unsigned int>::max();*/
	FILE *fin;
	fin = fopen("data/docs.txt", "r");
	char docId[MAX_FILE];
	ReadWord(docId, fin);
	cout<<docId<<" ";
	ReadWord(docId, fin);
	cout<<docId<<" ";
	char a = fgetc(fin);
	cout<<a<<" "<<(char)fgetc(fin)<<"\n";

	
}

