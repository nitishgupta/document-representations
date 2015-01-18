#!/usr/bin/env python
# -*- coding: utf-8 -*-
import nltk
from nltk.tokenize import sent_tokenize
from nltk.tokenize import RegexpTokenizer
import sys

'''
Tokenizes input document into sentences.
Tokenizes the sentences and
	Removes stranded integers
	Removes Punctuations
	NOT converting sentences to lower case. KEEPING sentences case sensitive. 	
'''

def readInputText(fi):
	inputText = ""
	for line in fi:
		inputText = inputText + line;
	try:
		inputText = inputText.decode('utf-8')
		print "Encoding : utf-8"
	except:
		print "Encoding : windows-1252"
		inputText = inputText.decode("windows-1252")
	return inputText;

def fileNames():
	if(len(sys.argv) > 2):
		inputFile = sys.argv[1]
		outFile = sys.argv[2]
	files = []
	files.append(inputFile)
	files.append(outFile)		
	return files

def processSentence(sentence):
	#sentence = sentence.lower()					# Keeping case-sensitive sentences
	tokenizer = RegexpTokenizer(r'\w+')
	tokens = tokenizer.tokenize(sentence)
	tokens = [token for token in tokens if not (token.isdigit() or token[0] == '-' and token[1:].isdigit())]
	return " ".join(tokens)

files = fileNames();
fi = open(files[0], 'r')
fo = open(files[1], 'w')

inputText = readInputText(fi);
sentences = sent_tokenize(inputText)
outText = ""
for sent in sentences:
	sent = processSentence(sent);
	outText = outText + sent.encode('utf-8') + "\n"
fo.write(outText.strip())

	
