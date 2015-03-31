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
#include <pthread.h>
#include <math.h>
#include "vector"
#include "map"
#include "set"
#include <pthread.h>

#include "util.hpp"


#define MAX_STRING 200
#define MAX_WORD 20
#define MAX_SENTENCE_LENGTH 1000

using namespace std;

typedef float real;                    // Precision of float numbers

struct vocab_word {
  long long cn;
  char *word;
  long long id;
};


long long train_words=0, vocab_size=0, vocab_max_size=100, words_processed = 0, perc = 0, docs_processed = 0;
int debug_mode = 0, min_count = 1, num_docs, embed_size=200, window=3, num_threads = 1, negative = 10;
int updateWeights = 1, Epoch = 1;
real learning_rate = 0.0025, starting_alpha, sample = -1, lambda = 0.1;
clock_t start;
//char word_outfile[MAX_STRING]; = "output/embeddings/words.dat";
//char doc_outfile[MAX_STRING] = "output/embeddings/docs.dat";

char train_file[MAX_STRING], train_directory[MAX_STRING], output_directory[MAX_STRING],  **traindocs, **docs, word_outfile[MAX_STRING], doc_outfile[MAX_STRING], doc_w_outfile[MAX_STRING];
struct vocab_word *vocab;
real *word_e, *doc_e, *nn_weight;

std::map<std::string, int> wordId;
std::map<std::string, int> docId;

const int table_size = 1e8;
int *table;

void printVec(real *vec){
	for(int i=0; i<embed_size; i++)
		cout<<vec[i]<<"\t";
	cout<<"\n";
}

void printMat(real *mat){
	cout<<"\n";
	for(int i=0; i<embed_size; i++){
		for(int j=0; j<embed_size; j++){
			cout<<mat[i*embed_size + j]<<"\t";
		}
		cout<<"\n";
	}
	cout<<"\n";
}

void InitUnigramTable() {
  int a, i;
  double train_words_pow = 0;
  double d1, power = 0.75;
  table = (int *)malloc(table_size * sizeof(int));
  for (a = 0; a < vocab_size; a++) train_words_pow += pow(vocab[a].cn, power);
  i = 0;
  d1 = pow(vocab[i].cn, power) / train_words_pow;
  for (a = 0; a < table_size; a++) {
    table[a] = i;
    if (a / (double)table_size > d1) {
      i++;
      d1 += pow(vocab[i].cn, power) / train_words_pow;
    }
    if (i >= vocab_size) i = vocab_size - 1;
  }
}

// Reads a word and returns its index in the vocabulary
string getWordFromIndex(int index){
	if(index < vocab_size && index >= 0)
		return vocab[index].word;
	else
		return "-1";
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

int ReadWordIndex(FILE *fin) {
  char word[MAX_STRING];
  ReadWord(word, fin);
  if (feof(fin)) return -1;
  return searchVocab(word);
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



/* Read words from file into Map<Word, Count> 
To prune, iterate through Map<Word, Count> deleting elements and copying only elements with count more than threshold to Map<Word, pair(wordId, count)>
Also keep creating Array[word], where arrayId = wordId
Total memory - Map<Word, pair(wordId, count)>, Array[word].  */
void learnVocabFromFile(){
	char word[MAX_STRING], docid[MAX_STRING];
	FILE *fin;
	long long a, i;
	vocab_size = 0;
	AddWordToVocab((char *)"</s>");
	for(int nd=0; nd<num_docs; nd++){
		fin = fopen(traindocs[nd], "rb");
		if (fin == NULL) {
			cout<<traindocs[nd]<<"\n";
			printf("ERROR: training doc file not found!\n");
			continue;
			//exit(1);
		}
		/*ReadWord(docid, fin);
		int d;
		d = searchDoc(docid);
		cout<<docid<<" "<<d<<"\n";
		if (fin == NULL || d == -1) {
			printf("ERROR: training doc file not found!\n");
			exit(1);
		}*/
		while (1) {
			ReadWord(word, fin);
			if (feof(fin)){ break; }
			if(strlen(word) <= 1)	continue;
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
		fclose(fin);
	}
	//SortVocab();
	printf("Vocab size: %lld\n", vocab_size);
	printf("Words in train file: %lld\n", train_words);
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

  	traindocs = (char **)calloc(num_docs, sizeof(char*));	// Contains the full address to train docs. Eg. docs/kafka.txt
  	docs = (char **)calloc(num_docs, sizeof(char*));		// Contains the full address to train docs. Eg. docs/kafka.txt
  	if(traindocs == NULL){cout<<"ERROR : NOT ENOUGH MEMORY TO ALLOCATE TRAIN DOCS LIST"; exit(1);}
  	if (dfile.is_open()) {
    	for(int i=0; i<num_docs; i++){
    		getline(dfile, filename, '\t');
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
	// if(debug_mode>0){
	// 	for(int i=0; i<num_docs; i++)
	// 		cout<<traindocs[i]<<"\n"<<docs[i]<<"\n";
	// }
}

void InitNet(){
	long long a,b,i,j;
	unsigned long long next_random = 1;
	// Allocating required memory for the word and doc embedding vectors
	a = posix_memalign((void **)&word_e, 128, (long long)vocab_size * embed_size * sizeof(real));
	if (word_e == NULL) {printf("Word Embeddings Memory allocation failed\n"); exit(1);}
	a = posix_memalign((void **)&doc_e, 128, (long long)num_docs * embed_size * sizeof(real));
	if (doc_e == NULL) {printf("Document Embedding Memory allocation failed\n"); exit(1);}
	
	// Initializing Word and Document Embeddings to small random numbers in range [-1/d, 1/d]
	for (a = 0; a < vocab_size; a++) for (b = 0; b < embed_size; b++) {
    	next_random = next_random * (unsigned long long)25214903917 + 11;
    	word_e[a * embed_size + b] = (((next_random & 0xFFFF) / (real)65536) - 0.5) / embed_size;
 	}
 	for (a = 0; a < num_docs; a++) for (b = 0; b < embed_size; b++) {
    	next_random = next_random * (unsigned long long)25214903917 + 11;
    	doc_e[a * embed_size + b] = (((next_random & 0xFFFF) / (real)65536) - 0.5) / embed_size;
 	}

 	// Allocating space for (2*window_size + 1) weight matrices, each of size d*d (embed-size * embed_size)
 	a = posix_memalign((void **)&nn_weight, 128, (long long)(2*window + 1) * sizeof(real));
 	if (nn_weight == NULL) {printf("Weight Matrices Memory allocation failed\n"); exit(1);}

	if(updateWeights > 0){
		// Initializing Neural Network Weights to 1 with Updates
		cout<<"Using Weights with Updates\n";
		for (a = 0; a < (2*window + 1); a++)
		nn_weight[a] = 1.0;
 	}
	else{
		// Initializing Neural Network Weights to 1 without Updates
		cout<<"Using Weights without Updates\n";
		for (a = 0; a < (2*window + 1); a++)
			nn_weight[a] = 1.0;
 	}
	

}

void printWeights(){
	for(int a=0; a<(2*window + 1); a++){
		cout<<a<<"\t:\t"<<nn_weight[a];
		cout<<"\n";
	}
}

void getnanword(int word, real *vec){
	for(int i =0; i<embed_size; i++){
		if(vec[i] > 1e+05){
			cout<<getWordFromIndex(word)<<"\n";
			return;			
		}
	}
}

void update(int center_word, int *context, int label){
	/* Calculate h_c and h_t */
	real *h_c, *vec, *matvec, weight;			// Corresponds to hidden vector found using doc and context, Sum(H'*w_t-k). 
	real *w_t = word_e + embed_size * center_word;		// Corresponds to the embedding of the middle word.
	//real *w_t = word_e + center_word;		// Corresponds to the embedding of the middle word.

	
	h_c = (real *)calloc(embed_size, sizeof(real));
	// Calculate h_c
	for(int i=0; i<2*window+1; i++){
		weight = nn_weight[i];
		if(i == 0){
			vec = doc_e + embed_size * context[i];
			//h_c = MatVec(matrix, vec, embed_size);
			scalarVec(vec, weight, h_c, embed_size);
		} else {
			vec = word_e + embed_size * context[i];
			addToVec(h_c, vec, weight, embed_size);
			//weighted_addVec(vec, weight, h_c, h_c, embed_size)
		}
	}

	real s = dotProd(w_t, h_c, embed_size);
	real estimated_prob = sigmoid(s);

	// Update for w_t
	updateVec(w_t, h_c, (label-estimated_prob), learning_rate, 0, embed_size);
	//addToVec(w_t, h_c, learning_rate*(label - estimated_prob), embed_size);

	//Update w_t-k's i.e. context doc and word embeddings
	for(int i=0; i<2*window+1; i++){
		weight = nn_weight[i];
		if(i == 0){
			vec = doc_e + embed_size * context[0];
			updateVec(vec, w_t, weight*(label-estimated_prob), learning_rate, 0, embed_size);
			//addToVec(vec, w_t, weight*learning_rate*(label - estimated_prob), embed_size);
		} else {
			vec = word_e + embed_size * context[i];
			updateVec(vec, w_t, weight*(label-estimated_prob), learning_rate, 0, embed_size);
			//addToVec(vec, w_t, weight*learning_rate*(label - estimated_prob), embed_size);
		}
	}	

	// Updates for context matrices
	if(updateWeights > 0){
		for(int i=1; i<2*window+1; i++){
			if(i==0)
				vec = doc_e + embed_size*context[i];
			else
				vec = word_e + embed_size*context[i];

			nn_weight[i] = nn_weight[i] + learning_rate*( (label - estimated_prob)*dotProd(vec, w_t, embed_size) - lambda*nn_weight[i]);
		}		
	}	

	free(h_c);
}


void *TrainModelThread(void *id){

	int sen[MAX_SENTENCE_LENGTH + 1], sentence_length = 0, word, sentence_position = 0;
	unsigned long long next_random = (long long)id;			// Change to id of the process
	int center_word, label = 1;

	int startdoc = ((long)id * num_docs)/num_threads;	// Setting start-doc index for this particular thread
	int enddoc = startdoc + (num_docs/num_threads);		// Setting end-doc index for this particular thread
	if((long)id == num_threads-1)						// Giving the last thread extra docs 
		enddoc = num_docs;
	for(int nd=startdoc; nd<enddoc; nd++){
		sentence_length=0; sentence_position=0; word = 0;
		//cout<<"\nThread "<<long(id)<<" "<<docs[nd]<<"\n";

		FILE *fin = fopen(traindocs[nd], "rb");			// Read the doc for learning
		while(1){				
			// If Sentence_Length = 0, a new sentence needs to be read, that will be processed before proceeding in the document.
			if (sentence_length == 0) {	
				while (1) {
			        word = ReadWordIndex(fin);
			        if (feof(fin)) break;
			        if (word == -1) continue;
			        //word_count++;
			        if (word == 0) break;
			        // The subsampling randomly discards frequent words while keeping the ranking same
			        if (sample > 0) {
			           real ran = (sqrt(vocab[word].cn / (sample * train_words)) + 1) * (sample * train_words) / vocab[word].cn;
			           next_random = next_random * (unsigned long long)25214903917 + 11;
			           if (ran < (next_random & 0xFFFF) / (real)65536)	continue;
			        }
			        sen[sentence_length] = word;
			        sentence_length++;
			        if (sentence_length >= MAX_SENTENCE_LENGTH) break;
		      	}
		      	sentence_position = window;		// sentence_position = window if we want to only start with words that have proper negative and positive window.
		    }
		    
		    // Process this sentence, i.e. extract all contexts and update.
			int *context = (int *) calloc(2*window + 1, sizeof(int));
			context[0] =  nd;	// First in context is doc_id 
			for(int i=1; i<=window; i++){
			 	context[i] = sen[sentence_position - window + i - 1];
			 	context[window + i] = sen[sentence_position + i];
			}
			word = sen[sentence_position];

			


			// Updates - Learning embeddings and NN Weights
			for(int i=0; i<negative + 1; i++){
				if(i == 0){			// Positive Sampling
					center_word = word;
					label = 1;
				} else {
					next_random = next_random * (unsigned long long)25214903917 + 11;
		            center_word = table[(next_random >> 16) % table_size];
		            if (center_word == 0) center_word = next_random % (vocab_size - 1) + 1;
		            if (center_word == word) continue;
		            label = 0;
				}
				update(center_word, context, label);
			}
			sentence_position++;
			words_processed++;

			// if(words_processed % (train_words/100) == 0){
			// 	perc++;
			// 	cout<<"\n"<<perc<<" % train words complete\n";
			// }
			// if(words_processed % 2000 == 0){
			// 	cout <<"2000 more done! \n";
			// }
			free(context);	
			if (sentence_position >= sentence_length - window)  {		// >= sentence_length-window if we want to end early on a word with relevant positive side context. 
	  			//cout<<"Sentence Done!";
	  			sentence_length = 0;
	  			if(feof(fin))	break;				// If sentence is finished, check is file is also finished. If yes, move on to next doc.
	  			else 			continue;			// Else, continue to learn new sentences from the same doc.	
			}
		}
		fclose(fin);
		docs_processed++;
		if(docs_processed % 100 == 0)
			cout<<"Docs Processed : "<<docs_processed<<" thread : "<<(long)id<<"\n";
	}

	pthread_exit(NULL);
}

void writeEmbeddings(){
	long a,b,c,d;
	FILE *word_fo, *doc_fo, *doc_w_fo;

	strncpy(word_outfile, output_directory, MAX_STRING); strcat(word_outfile, "embeddings/word.dat");
  	strncpy(doc_outfile, output_directory, MAX_STRING); strcat(doc_outfile, "embeddings/doc.dat");
  	word_fo = fopen(word_outfile, "wb");
  	doc_fo = fopen(doc_outfile, "wb");

    // Save the word and doc vectors
    fprintf(word_fo, "%lld\t%d\n", vocab_size, embed_size);
    fprintf(doc_fo, "%d\t%d\n", num_docs, embed_size);

    for (a = 0; a < vocab_size; a++) {
		if(a < num_docs){	
			fprintf(word_fo, "%s", vocab[a].word);		// Print word
			fprintf(doc_fo, "%s", docs[a]);		// Print Doc

			for (b = 0; b < embed_size; b++){ 
				fprintf(word_fo, "\t%lf", word_e[a * embed_size + b]);
				fprintf(doc_fo, "\t%lf", doc_e[a * embed_size + b]);
			}
			fprintf(word_fo, "\n");
			fprintf(doc_fo, "\n");
		} else {
			fprintf(word_fo, "%s ", vocab[a].word);		// Print word
			for (b = 0; b < embed_size; b++){ 
				fprintf(word_fo, "\t%lf ", word_e[a * embed_size + b]);
			}	
			fprintf(word_fo, "\n");
		}
    }
    fclose(word_fo);
    fclose(doc_fo);
}

void TrainModel(){
	long a,b,c,d;
	
	getTrainingFileNames();
	//cout<<"asdfasdfasdf \n\n";
	print_traindocs();
	
	fflush(stdout);
	learnVocabFromFile();
	printf("Vocab Learnt\n");
	printVocab();

	pthread_t *pt = (pthread_t *)malloc(num_threads * sizeof(pthread_t));
	InitNet();
	
	InitUnigramTable();

	for(int epoch = 0; epoch < Epoch; epoch++){
		cout<<"\n EPOCH : "<<epoch<<"\n\n";
		docs_processed = 0;
		for (a = 0; a < num_threads; a++) pthread_create(&pt[a], NULL, TrainModelThread, (void *)a);
	  	for (a = 0; a < num_threads; a++) pthread_join(pt[a], NULL);
	}
  	writeEmbeddings();	
  	printWeights();
}



int main(int argc, char **argv){
	int i, pvocab=0;
	cout<<"Word and Document Embeddings - Nitish"<<"\n";
	if ((i = ArgPos((char *)"-train-directory", argc, argv)) > 0){ 
		strcpy(train_directory, argv[i + 1]); 
		if(train_directory[strlen(train_directory)-1]!='/'){
			train_directory[strlen(train_directory)]='/';
			train_directory[strlen(train_directory)+1]='\0';
		}
	}
	if ((i = ArgPos((char *)"-output-directory", argc, argv)) > 0){ 
		strcpy(output_directory, argv[i + 1]); 
		if(output_directory[strlen(output_directory)-1]!='/'){
			output_directory[strlen(output_directory)]='/';
			output_directory[strlen(output_directory)+1]='\0';
		}
	}
	if ((i = ArgPos((char *)"-debug", argc, argv)) > 0) debug_mode =  atoi(argv[i + 1]);
	if ((i = ArgPos((char *)"-min-count", argc, argv)) > 0)  min_count =  atoi(argv[i + 1]);
	if ((i = ArgPos((char *)"-embed-size", argc, argv)) > 0)  embed_size =  atoi(argv[i + 1]);
	if ((i = ArgPos((char *)"-window-size", argc, argv)) > 0)  window =  atoi(argv[i + 1]);
	if ((i = ArgPos((char *)"-nthreads", argc, argv)) > 0)  num_threads =  atoi(argv[i + 1]);
	if ((i = ArgPos((char *)"-sample", argc, argv)) > 0) sample = atof(argv[i + 1]);
	if ((i = ArgPos((char *)"-negative-samples", argc, argv)) > 0) negative = atoi(argv[i + 1]);
	if ((i = ArgPos((char *)"-update-weights", argc, argv)) > 0) updateWeights = atoi(argv[i + 1]);
	if ((i = ArgPos((char *)"-epoch", argc, argv)) > 0) Epoch = atoi(argv[i + 1]);
	if ((i = ArgPos((char *)"-lr", argc, argv)) > 0) learning_rate = atof(argv[i + 1]);

	vocab = (struct vocab_word *)calloc(vocab_max_size, sizeof(struct vocab_word));
	TrainModel();
	return 0;	

}
