1. preprocess.py - Takes in document file and outputs a processed version, that is tokenized, ripped off punctuations and numbers. The doc is reduced to one line text that is space separated.

2. Create a file that contains all docs in a \n or \t separated format. A separate file, containing the docIds in order will be stored.

3. vocab.cpp - Should take in the file containing all the Docs, and create the vocab i.e. a Map<Word, Int> and a Array of Ints[Words]. It should contain methods to reduce the size of the vocab wrt to the unigram frequency, store the unigram frequency and store the word vectors associated with each word.