// See arguments from main()
// vocab_size - Size of the vocab. Final number is after removal of words based on min_count
// num_docs - Number of docs to be used for learning.
// train_directory - Directoy containing
//						- docs.txt - file with first line as number of documents and subsequent lines as filenames of docs to be learnt
//						- docs to be learnt
// traindocs - is an array of the addresses of the docs to be learnt
// vocab - array of words in vocab. The array ID acts as int proxy for word.
// wordId - reverse map from word-> intId for words in vocab.

/*
@author nitish
@since 1/17/15
*/

#include <iostream>
#include <fstream>
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


long long train_words=0, vocab_size=0, vocab_max_size=100;
int debug_mode = 0, min_count = 1, num_docs;
char train_file[MAX_STRING];
char train_directory[MAX_STRING];
char **traindocs;
char **docs;
struct vocab_word *vocab;

std::map<std::string, int> wordId;
std::map<std::string, int> docId;




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

int searchDoc(char *docid){
	std::map<std::string, int>::iterator loc = docId.find(docid);
	if(loc != docId.end())	return loc->second;
	else return -1;	
}

/* Read words from file into Map<Word, Count> 
To prune, iterate through Map<Word, Count> deleting elements and copying only elements with count more than threshold to Map<Word, pair(wordId, count)>
Also keep creating Array[word], where arrayId = wordId
Total memory - Map<Word, pair(wordId, count)>, Array[word].  
*/
void learnVocabFromFile(){
	char word[MAX_STRING], docid[MAX_STRING];
	FILE *fin;
	long long a, i;
	vocab_size = 0;
	AddWordToVocab((char *)"</s>");
	for(int nd=0; nd<num_docs; nd++){
		fin = fopen(traindocs[nd], "rb");
		ReadWord(docid, fin);
		int d;
		d = searchDoc(docid);
		cout<<docid<<" "<<d<<"\n";
		if (fin == NULL || d == -1) {
			printf("ERROR: training doc file not found!\n");
			exit(1);
		}
		while (1) {
			ReadWord(word, fin);
			if (feof(fin)){ break; }
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
	}
	//SortVocab();
	printf("Vocab size: %lld\n", vocab_size);
	printf("Words in train file: %lld\n", train_words);
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

void getTrainingFileNames(){
	string docs_file = train_directory, filename, fileid, line;
	docs_file.append("/docs.txt");
	ifstream dfile (docs_file);
	if (dfile.is_open()) {
  		getline(dfile, line);
  		num_docs = stoi(line, NULL, 10);
  	}
  	else{
  		cout<<"ERROR : FILE WITH DOC NAMES NOT FOUND";
  		exit(1);
  	}

  	traindocs = (char **)calloc(num_docs, sizeof(char*));
  	docs = (char **)calloc(num_docs, sizeof(char*));
  	if(traindocs == NULL){cout<<"ERROR : NOT ENOUGH MEMORY TO ALLOCATE TRAIN DOCS LIST"; exit(1);}
  	if (dfile.is_open()) {
    	for(int i=0; i<num_docs; i++){
    		getline(dfile, filename, ' ');
    		getline(dfile, fileid);
    		string file_address = train_directory;
    		file_address.append(filename);

    		traindocs[i] = (char *)malloc(file_address.size()+1);
    		std::copy(file_address.begin(), file_address.end(), traindocs[i]);
    		traindocs[i][file_address.size()] = '\0';

    		docs[i] = (char *)malloc(fileid.size()+1);
    		std::copy(fileid.begin(), fileid.end(), docs[i]);
    		docs[i][fileid.size()] = '\0';
    		docId[fileid] = i;
    	}
    	dfile.close();
  	}
}

void print_traindocs(){
	cout<<"Train Directory : "<<train_directory<<"\n";
	cout<<"number of docs : "<<num_docs<<"\n";
	if(debug_mode>0){
		for(int i=0; i<num_docs; i++)
			cout<<traindocs[i]<<"\n"<<docs[i]<<"\n";
	}
}

int main(int argc, char **argv){
	int i, pvocab=0;
	cout<<"VOCAB"<<"\n";
	if ((i = ArgPos((char *)"-train-directory", argc, argv)) > 0){ 
		strcpy(train_directory, argv[i + 1]); 
		if(train_directory[strlen(train_directory)-1]!='/'){
			train_directory[strlen(train_directory)]='/';
			train_directory[strlen(train_directory)+1]='\0';
		}
	}
	if ((i = ArgPos((char *)"-debug", argc, argv)) > 0) debug_mode =  atoi(argv[i + 1]);
	if ((i = ArgPos((char *)"-min-count", argc, argv)) > 0)  min_count =  atoi(argv[i + 1]);
	vocab = (struct vocab_word *)calloc(vocab_max_size, sizeof(struct vocab_word));
	getTrainingFileNames();
	print_traindocs();
	learnVocabFromFile();
	printVocab();

	return 0;	

}
