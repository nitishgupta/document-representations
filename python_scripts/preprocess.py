#!/usr/bin/env python
# -*- coding: utf-8 -*-
import nltk
from nltk.tokenize import sent_tokenize
from nltk.tokenize import RegexpTokenizer
import sys
'''
@author nitish
@since 1/17/15
'''
'''
argument 1 : input file, argument 2 : output file
Tokenizes input document into sentences.
Tokenizes the sentences and
	Removes stranded integers
	Removes Punctuations
	NOT converting sentences to lower case. KEEPING sentences case sensitive. 	
'''

def readInputText(fi):
	#inputText = ""
	inputText = fi.read()
	#print "Hello"
	#for line in fi:
	#	inputText = inputText + line;
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

# tokenizes and removes punctuations. removes stranded numbers. joins using space.
def processText(text):
	#sentence = sentence.lower()					# Keeping case-sensitive sentences
	tokenizer = RegexpTokenizer('\w+|\d.+')
	tokens = tokenizer.tokenize(text)
	tokens_processed = []
	for token in tokens:
		if (token.isdigit() or (token[0] == '-' and token[1:].isdigit())):
			tokens_processed.append("number")
		else:
			tokens_processed.append(token)

	#tokens = [token for token in tokens if not (token.isdigit() or token[0] == '-' and token[1:].isdigit())]
	#return " ".join(tokens)
	return " ".join(tokens_processed)

# Output - join sentences using tab delimiter
def writeOutput(intext, fo):
	outText = ""
	outText = processText(intext);
	outText = outText.encode('utf-8')
	fo.write(outText.strip())


if(__name__=="__main__"):
	files = fileNames();
	fi = open(files[0], 'r')
	fo = open(files[1], 'w')

	inputText = readInputText(fi);
	writeOutput(inputText, fo);

	
