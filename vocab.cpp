//argument 1 : input file, argument 2 : output file
//Tokenizes input document into sentences.
//Tokenizes the sentences and
//	Removes stranded integers
//	Removes Punctuations
//	NOT converting sentences to lower case. KEEPING sentences case sensitive. 	

/*
@author nitish
@since 1/17/15
*/

#include <iostream>
#include <stdio.h>
#include <string.h>
#include "vector"
#include "map"
#include "set"
#include <pthread.h>

#define MAX_STRING 100
#define VOCAB_SIZE 1000000

using namespace std;

struct vocab_word {
  long long cn;
  char *word;
  long long id;
};

/*
Read words from file into Map<Word, Count>
To prune, iterate through Map<Word, Count> deleting elements and copying only elements with count more than threshold to Map<Word, pair(wordId, count)>
Also keep creating Array[word], where arrayId = wordId
Total memory - Map<Word, pair(wordId, count)>, Array[word].  
*/
void readVocabFromFile(){

}

int main(){
	cout<<"nitish\n";
	return 0;	

}
