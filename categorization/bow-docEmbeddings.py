import sys
import numpy as np
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.feature_extraction.text import TfidfVectorizer
import scipy.sparse 
import itertools


def writeEmbeddings(phi_docs_nonsparse, doc_names, out_filename):
	fo = open(out_filename, 'w')
	num_docs = phi_docs_nonsparse.shape[0]
	embed_size = phi_docs_nonsparse.shape[1]
	print "Num Docs : ", num_docs, " Embed Size : ", embed_size
	fo.write(str(num_docs) + "\t" + str(embed_size) + "\n")

	doc_id = 0;
	for doc in doc_names:
		fo.write(doc)
		for k in range(0, embed_size):
			fo.write("\t" + str(phi_docs_nonsparse[doc_id, k]))
		doc_id += 1
		fo.write("\n")
	if(doc_id != num_docs):
		print "Some Error"
		sys.exit()

	fo.close()

def writeEmbeddings_SVD(phi_doc_svd, doc_names, K, out_filename):
	fo = open(out_filename, 'w')
	num_docs = phi_doc_svd.shape[0]
	embed_size = K
	print "Num Docs : ", num_docs, " Embed Size : ", embed_size
	fo.write(str(num_docs) + "\t" + str(embed_size) + "\n")

	doc_id = 0;
	for doc in doc_names:
		fo.write(doc)
		for k in range(0, embed_size):
			fo.write("\t" + str(phi_doc_svd[doc_id, k]))
		doc_id += 1
		fo.write("\n")
	if(doc_id != num_docs):
		print "Some Error"
		sys.exit()

	fo.close()


			


def doc_addrs(docs_input_file):
	fi = open(docs_input_file, 'r')
	num_docs = fi.readline().strip()
	doc_addresses = []
	doc_names = []
	for line in fi:
		doc_addr, doc_name = doc_addr_and_name(line, docs_input_directory)
		doc_addresses.append(doc_addr)
		doc_names.append(doc_name)

	return doc_addresses, doc_names	

def doc_addr_and_name(line, docs_input_directory):
	a = line.split("\t")
	if(len(a) > 1):
		doc_addr = docs_input_directory + a[0].strip()
		doc_name = a[1].strip()
	else:
		print "Error"
		sys.exit()

	return doc_addr, doc_name	

def using_tocoo_izip(x):
    cx = x.tocoo()
    print len(cx.col)
    for i,j,v in itertools.izip(cx.row, cx.col, cx.data):
    	pass
     	#if( i == 0):
        # 	print (i,j,v)

if __name__=="__main__":
	docs_input_directory = sys.argv[1]
	if(docs_input_directory[-1] != "/"):
		docs_input_directory += "/"

	docs_input_file = docs_input_directory + "docs.txt"

	embeddings_directory = sys.argv[2]
	if(embeddings_directory[-1] != "/"):
		embeddings_directory += "/"

	output_file = embeddings_directory + "embeddings/" + "doc.dat"

	doc_addresses, doc_names = doc_addrs(docs_input_file)

	#vectorizer = CountVectorizer(input='filename', analyzer='word', lowercase='False', binary="True")
	#phi_docs = vectorizer.fit_transform(doc_addresses)
	
	vectorizer = TfidfVectorizer(input='filename', analyzer='word', lowercase='False', stop_words='english')
	phi_docs = vectorizer.fit_transform(doc_addresses)	

	max_num_docs = phi_docs.shape[0] - 1
	Max_vocab_size = phi_docs.shape[1] - 1

	phi_docs_nonsparse = phi_docs.todense()

	if(sys.argv[3]):
		print "Doc-Term matrix computed, starting to perform SVD."
		doc_embed, s, term_embed = np.linalg.svd(phi_docs_nonsparse, full_matrices=False)
		print "SVD Performed"
		print np.dot(doc_embed[100], doc_embed[200])
		#writeEmbeddings_SVD(doc_embed, doc_names, int(sys.argv[3]), output_file)
	else:
		writeEmbeddings(phi_docs_nonsparse, doc_names, output_file)
