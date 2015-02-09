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
#define MAX_WORD 20

using namespace std;

struct vocab_word {
  long long cn;
  char *word;
  long long id;
};


long long train_words=0, vocab_size=0, vocab_max_size=1000, file_size;
int debug_mode = 0, min_count = 1;
char train_file[MAX_STRING];
struct vocab_word *vocab;
std::map<std::string, int> wordId;



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

int VocabCompare(const void *a, const void *b) {
    return ((struct vocab_word *)b)->cn - ((struct vocab_word *)a)->cn;
}

void SortVocab() {
  int a, size;
  // Sort the vocabulary and keep </s> at the first position
  qsort(&vocab[1], vocab_size - 1, sizeof(struct vocab_word), VocabCompare);
  // Delete the wholw word->id map and recompute Ids.
  wordId.clear();
  wordId[vocab[0].word] = 0;
  size = vocab_size;
  for(int i=1; i<size; i++){
  	if(vocab[i].cn <= min_count){
  		vocab_size--;
  		free(vocab[i].word);
  	} else {
  		vocab[i].id = i;
  		wordId[vocab[i].word] = i;
  	}
  }
}  


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

int AddWordToVocab(char* word){
	unsigned int length = strlen(word)+1;
	if (length > MAX_STRING) length = MAX_STRING;
	// Add word to vocab - struct vocab_word
	vocab[vocab_size].word = (char *)calloc(length, sizeof(char));
	strcpy(vocab[vocab_size].word, word);
	vocab[vocab_size].cn = 0;
	vocab[vocab_size].id = vocab_size;
	//Add word to wordId - Map
	wordId[word] = vocab_size;

	vocab_size++;
	// Reallocate memory if needed
	if (vocab_size + 2 >= vocab_max_size) {
	vocab_max_size += 1000;
	vocab = (struct vocab_word *)realloc(vocab, vocab_max_size * sizeof(struct vocab_word));
	}
	return vocab_size - 1;
}	

int searchVocab(char *word){
	std::map<std::string, int>::iterator loc = wordId.find(word);
	if(loc != wordId.end())	return loc->second;
	else return -1;
}

/* Read words from file into Map<Word, Count> 
To prune, iterate through Map<Word, Count> deleting elements and copying only elements with count more than threshold to Map<Word, pair(wordId, count)>
Also keep creating Array[word], where arrayId = wordId
Total memory - Map<Word, pair(wordId, count)>, Array[word].  
*/
void learnVocabFromFile(){
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
		i = searchVocab(word);
    	if (i == -1) {
      		a = AddWordToVocab(word);
      		vocab[a].cn = 1;
    	} else vocab[i].cn++;
	}
	//SortVocab();
	printf("Vocab size: %lld\n", vocab_size);
	printf("Words in train file: %lld\n", train_words);
	file_size = ftell(fin);
	fclose(fin);
	SortVocab();
}

void printVocab(){
	string w;
	if(debug_mode>0){
		for (int i=0; i<vocab_size; i++)
			cout<<vocab[i].word<<"->"<<vocab[i].cn<<"->"<<vocab[i].id<<"\n";
	}
	cout<<"Vocab Size : "<<vocab_size<<"\n";
	cout<<"Training Words : "<<train_words<<"\n";
	/*int total_word = 0;
	for (std::map<string,int>::iterator it=wordCount.begin(); it!=wordCount.end(); ++it){
		if(debug_mode>0)	std::cout << it->first << " => " << it->second << '\n';
		total_word += it->second;
	}
	cout<<"Total Words : "<<total_word<<"\n";
	cout<<"File Size : "<<file_size<<"\n";*/
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
	int i, pvocab=0;
	cout<<"nitish"<<"\n";
	if ((i = ArgPos((char *)"-train", argc, argv)) > 0) strcpy(train_file, argv[i + 1]);
	if ((i = ArgPos((char *)"-debug", argc, argv)) > 0) debug_mode =  atoi(argv[i + 1]);
	if ((i = ArgPos((char *)"-min-count", argc, argv)) > 0)  min_count =  atoi(argv[i + 1]);
	vocab = (struct vocab_word *)calloc(vocab_max_size, sizeof(struct vocab_word));
	learnVocabFromFile();
	printVocab();
	return 0;	

}
