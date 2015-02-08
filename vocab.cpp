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
#define VOCAB_SIZE 10000000
#define MAX_WORD 20

long long train_words=0, vocab_size=0;
int debug_mode = 0;
char train_file[MAX_STRING];
long long file_size;
std::map<std::string, int> wordCount;
std:map<std::string, std::pair<long long, long long>> widc;
char[VOCAB_SIZE][MAX_WORD] idWord;


using namespace std;

struct vocab_word {
  long long cn;
  char *word;
  long long id;
};



/*void ReadWord(char *word, FILE *fin) {
  int a = 0, ch;
  while (!feof(fin)) {
    ch = fgetc(fin);
    if (ch == 13) continue;
    if ((ch == ' ') || (ch == '\t') || (ch == '\n')) {
      if (a > 0) {
        if (ch == '\n') ungetc(ch, fin);
        break;
      }
      if (ch == '\n') {
        strcpy(word, (char *)"</s>");
        return;
      } else continue;
    }
    word[a] = ch;
    a++;
    if (a >= MAX_STRING - 1) a--;   // Truncate too long words
  }
  word[a] = 0;
}*/

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

void AddWordToVocab(string word){
	std::map<std::string, int>::iterator loc = wordCount.find(word);
	if(loc != wordCount.end()){
		int cc = loc->second;
		cc++;
		wordCount[word] = cc;
	}
	else{
		wordCount[word] = 1;
		vocab_size++;
	}
}

/* Read words from file into Map<Word, Count> 
To prune, iterate through Map<Word, Count> deleting elements and copying only elements with count more than threshold to Map<Word, pair(wordId, count)>
Also keep creating Array[word], where arrayId = wordId
Total memory - Map<Word, pair(wordId, count)>, Array[word].  
*/
void readVocabFromFile(){
	char word[MAX_STRING];
	FILE *fin;
	long long a, i;
	//for (a = 0; a < vocab_hash_size; a++) vocab_hash[a] = -1;
	fin = fopen(train_file, "rb");
	if (fin == NULL) {
		printf("ERROR: training data file not found!\n");
		exit(1);
	}
	vocab_size = 0;
	AddWordToVocab((char *)"</s>");
	while (1) {
		ReadWord(word, fin);
		if (feof(fin)) break;
		train_words++;
		//cout<<train_words;
		if ((debug_mode > 0) && (train_words % 10 == 0)) {
	  		printf("%lldK%c", train_words / 10, 13);
		  	fflush(stdout);
		}
		AddWordToVocab(word);
	}
	//SortVocab();
	printf("Vocab size: %lld\n", vocab_size);
	printf("Words in train file: %lld\n", train_words);
	file_size = ftell(fin);
	fclose(fin);
}

void printVocab(){
	int total_word = 0;
	for (std::map<string,int>::iterator it=wordCount.begin(); it!=wordCount.end(); ++it){
		if(debug_mode>0)	std::cout << it->first << " => " << it->second << '\n';
		total_word += it->second;
	}
	cout<<"Total Words : "<<total_word<<"\n";
	cout<<"File Size : "<<file_size<<"\n";
}

int ArgPos(char *str, int argc, char **argv) {
  int a;
  for (a = 1; a < argc; a++) if (!strcmp(str, argv[a])) {
    if (a == argc - 1) {
      printf("Argument missing for %s\n", str);
      exit(1);
    }
    return a;
  }
  return -1;
}

int main(int argc, char **argv){
	if(idWord != NULL) cout << "space allocated";
	int i, pvocab=0;
	cout<<"nitish"<<"\n";
	if ((i = ArgPos((char *)"-train", argc, argv)) > 0) strcpy(train_file, argv[i + 1]);
	if ((i = ArgPos((char *)"-debug", argc, argv)) > 0) debug_mode =  atoi(argv[i + 1]);
	readVocabFromFile();
	//printVocab();
	return 0;	

}
