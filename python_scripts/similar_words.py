import sys 
import numpy as np
import theano.tensor as T
from theano import *

def getWordSize(lines):
	a = lines[0].split("\t")
	words = int(a[0])
	size = int(a[1])
	ws = []
	ws.append(words)
	ws.append(size)
	return ws


def wordVec(lines, words, size):
	wordVec = {}
	for i in range(1, len(lines)):
		wv = lines[i].split("\t")
		word = wv[0].strip()
		vec = np.empty(size, dtype=float)
		for j in range(1, size + 1):
			v = float(wv[j].strip())
			vec[j-1] = v
		vec = vec/np.linalg.norm(vec)  # Normalization 	
		wordVec[word] = vec	
	return wordVec;	

def cosDis(vec1, vec2):
	return np.dot(vec1, vec2)	

def cosineDistance(vec1, vec2):
	v1 = T.fvector('v1')
	v2 = T.fvector('v2')
	cd = T.dot(v1, v2)
	f = function([v1, v2], cd, allow_input_downcast=True)
	return f(vec1, vec2)
	

def closestWords(i_word, wordVec, nwords):
	dd = {}
	for word in wordVec:
		#if(word != i_word):
		#dd[word] = cosineDistance(wordVec[word], wordVec[i_word])
		dd[word] = cosDis(wordVec[word], wordVec[i_word])
	sort = sorted(dd, key=dd.__getitem__, reverse=True)
	return sort[0:nwords]

		


input_file = sys.argv[1];
fi = open(input_file, 'r');
lines = fi.readlines()
words = getWordSize(lines)[0]
size = getWordSize(lines)[1]
wordVec = wordVec(lines, words, size);

print size
print closestWords(sys.argv[2], wordVec, 5)


