import sys 
import numpy as np
import theano.tensor as T
from theano import *

def getEntityNumandSize(lines):
	a = lines[0].split("\t")
	words = int(a[0])
	size = int(a[1])
	ws = []
	ws.append(words)
	ws.append(size)
	return ws


def entityVecs(lines, entities, size):
	enVec = {}
	for i in range(1, len(lines)):
		wv = lines[i].split("\t")
		en = wv[0].strip()
		vec = np.empty(size, dtype=float)
		for j in range(1, size + 1):
			v = float(wv[j].strip())
			vec[j-1] = v
		vec = vec/np.linalg.norm(vec)  # Normalization 	
		enVec[en] = vec	
	return enVec;	

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

def closestEntities2(en1, enVec1, enVec2, nen2):
	dd = {}
	for en2 in enVec2:
		dd[en2] = cosDis(enVec1[en1], enVec2[en2])
	sort = sorted(dd, key=dd.__getitem__, reverse=True)
	return sort[0:nen2]	


def getEntityVectors(input_file):		
	fi = open(input_file, 'r');
	lines = fi.readlines()
	ws = getEntityNumandSize(lines)
	entities = ws[0]
	size = ws[1]
	enVec = entityVecs(lines, entities, size);
	return enVec

input_file1 = sys.argv[1];
input_file2 = sys.argv[2];
enVec1 = getEntityVectors(input_file1)
enVec2 = getEntityVectors(input_file2)
en1 = sys.argv[3]
print closestEntities2(en1, enVec1, enVec2, 20)

