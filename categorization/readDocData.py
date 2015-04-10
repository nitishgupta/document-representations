# Get as input the filename containing Document	Category data
# Return proxies for Documents -> Int, Category -> Int
# Return the data as a list containing [Doc_proxy, Cat_proxy] pairs
#
from collections import defaultdict
import sys
from sys import stdout
import operator
import numpy as np
from sets import Set

def read(filename):
	data_points_read = 0
	docs_read = 0
	cats_read = 0
	fi = open(filename, 'r')
	docs = {}
	cats = {}
	catCount = defaultdict(int)
	data = []
	for line in fi:
		a = line.split("\t", 1)
		if(len(a) > 1):
			doc = a[0].strip()
			try:
				val = docs[doc]
			except KeyError:
				docs[doc] = docs_read
				docs_read += 1

			categories = a[1].split("\t")
			for c in categories:
				c = c.strip()
				try:
					val = cats[c]
				except KeyError:
					cats[c] = cats_read
					cats_read += 1
				catCount[cats[c]] += 1
				#catCount[c] += 1
				data_points_read = data_points_read + 1
				data.append([docs[doc], cats[c], 1])

		if(docs_read % 1000 == 0):
			sys.stdout.write(str(docs_read) + ", ")
	print "\nDocs : ", docs_read, "Categories : ", cats_read, "Data Points : ", len(data) 
	return docs, cats, catCount, data

def read_Reuters(filename, train_id_file, test_id_file):
	train_id = Set([])
	test_id = Set([])

	fi = open(train_id_file, 'r')
	for line in fi:
		train_id.add(line.strip())
	fi.close()
	fi = open(test_id_file, 'r')
	for line in fi:
		test_id.add(line.strip())
	fi.close()
	
	print "Train Ids : ", len(train_id), " TEst Ids : ", len(test_id)

	data_points_read = 0
	docs_read = 0
	cats_read = 0
	fi = open(filename, 'r')
	docs = {}
	cats = {}
	catCount = defaultdict(int)
	tr_v_data = []
	testdata = []
	for line in fi:
		a = line.split("\t", 1)
		if(len(a) > 1):
			doc = a[0].strip()
			try:
				val = docs[doc]
			except KeyError:
				docs[doc] = docs_read
				docs_read += 1

			categories = a[1].split("\t")
			for c in categories:
				c = c.strip()
				try:
					val = cats[c]
				except KeyError:
					cats[c] = cats_read
					cats_read += 1
				catCount[cats[c]] += 1
				#catCount[c] += 1
				data_points_read = data_points_read + 1
				if(doc in train_id):
					tr_v_data.append([docs[doc], cats[c], 1])
				elif(doc in test_id):
					testdata.append([docs[doc], cats[c], 1])

		if(docs_read % 1000 == 0):
			sys.stdout.write(str(docs_read) + ", ")
	print "\nDocs : ", docs_read, "Categories : ", cats_read, "Train Data Points : ", len(tr_v_data),  "Test Data Points : ", len(testdata)
	return docs, cats, catCount, tr_v_data, testdata


def read_Phi_Docs(filename, docs):
	fi = open(filename, 'r')
	line = fi.readline()
	print line
	num_docs = int(line.split("\t")[0].strip())
	print "Num Docs with Embeddings : ", num_docs, " in Doc Category Data : ", len(docs.keys())
	K = int(line.split("\t")[1].strip())
	if(num_docs != len(docs.keys())):
		print "WARNING : Docs in input is not equal to num of docs with embeddings";
	phi_docs = 	np.zeros(shape=(len(docs.keys()), K))
	for line in fi:
		a = line.split("\t", 1)
		if(len(a) > 1):
			doc_name = a[0].strip()
			try:
				doc_id = docs[doc_name]
				phi_d = a[1].split("\t")
				if(len(phi_d) != K):
					print "Error in K"
					sys.exit()
				else:
					phi_docs[doc_id] = np.array(phi_d)	
			except KeyError:
				print "Doc : ", doc_name, " not found"
				sys.exit()	

	print "K : ", K			
	return phi_docs, K			

def readWordsAndEmbeddings(word_embedding_inputfile):
	words = {}
	words_count = 0
	fi = open(word_embedding_inputfile, 'r')
	details = fi.readline()
	
	num_words = int(details.split("\t")[0].strip())
	K = int(details.split("\t")[1].strip())

	phi_words = np.zeros(shape=(num_words, K))

	for line in fi:
		a = line.split("\t", 1)
		if(len(a) > 1):
			word = a[0].strip()
			try:
				word_id = words[word]
				phi_word = a[1].split("\t")
				if(len(phi_word) != K):
					print "Error in K"
					sys.exit()
				else:
					phi_words[word_id] = np.array(phi_word)
			except:
				words[word] = words_count
				phi_word = a[1].split("\t")
				if(len(phi_word) != K):
					print "Error in K"
					sys.exit()
				else:
					phi_words[words_count] = np.array(phi_word)
				words_count += 1
	

	fi.close()
	return words, phi_words, K			
					


def readYelp(filename):
	data_points_read = 0
	buss_read = 0
	atts_read = 0
	fi = open(filename, "r")
	buss = {}
	atts = {}
	data = []
	bus_id = ""
	att = ""
	for line in fi:
		a = line.split(":")
		if(len(a) >= 2):
			if(a[0].strip() == "business_id"):
				bus_id = a[1].strip();
				#buss[bus_id] = buss_read	
				#buss_read = buss_read + 1
			else:
				att = a[0].strip()
				try:
					val = atts[att]
				except KeyError:
					atts[att] = atts_read
					atts_read += 1

				try:
					val = buss[bus_id]
				except KeyError:
					buss[bus_id] = buss_read
					buss_read += 1		
				
				truth = int(a[1].strip())
				data.append( [buss[bus_id], atts[att], truth] )
				data_points_read = data_points_read + 1
		if(data_points_read % 10000 == 0):
			sys.stdout.write(str(data_points_read) + ", ")
	print "\n"	
	return buss, atts, data		

def readAmazon(filename):
	data_points_read = 0
	buss_read = 0
	atts_read = 0
	fi = open(filename, "r")
	buss = defaultdict()
	atts = defaultdict()
	data = []
	for line in fi:
		a = line.split(" ", 3)
		if(len(a) >= 2):
			bus = a[0].strip()
			user = a[1].strip()
			rate = float(a[2].strip())
			if (rate >= 4):
				rate = 1
			else:
				rate = 0

			if bus not in buss.keys():
				buss[bus] = buss_read
				buss_read = buss_read + 1	
			if user not in atts.keys():
				atts[user] = atts_read
				atts_read = atts_read + 1			


			data.append( [buss[bus], atts[user], rate] )
			data_points_read = data_points_read + 1
			if(data_points_read % 1000 == 0):
				print data_points_read

	return buss, atts, data			




	

if __name__=="__main__":
	input_file = sys.argv[1]
	#file_d_embeddings = sys.argv[2]
	
	words, phi_words = readWordsAndEmbeddings(input_file)
	print len(words.keys()), words["Biology"]


	# docs, cats, catC, data = read(input_file);
	# phi_d, K = read_Phi_Docs(file_d_embeddings, docs)
	#print max(catC.iteritems(), key=operator.itemgetter(1))[0]





