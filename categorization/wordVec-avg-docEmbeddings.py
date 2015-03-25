import sys
import numpy as np
import readDocData


def doc_embedding(docs_input_directory, docs_input_file, words, phi_words, K, output_file):
	fi = open(docs_input_file, 'r')
	fo = open(output_file, 'w')
	num_docs = fi.readline().strip()
	fo.write(num_docs + "\t" + str(K) + "\n")

	for line in fi:
		doc_addr, doc_name = doc_addr_and_name(line, docs_input_directory)
		doc = open(doc_addr, 'r')
		text = doc.read()
		text_words = text.split(" ")
		words_averaged = 0
		phi_doc = np.zeros(shape=K)
		for word in text_words:
			try:
				word_id = words[word.strip()]
				phi_doc += phi_words[word_id]
				words_averaged += 1

			except KeyError:
				pass

		fo.write(doc_name)
		for k in range(0, K):
			fo.write("\t" + str(phi_doc[k]))

		fo.write("\n")
		doc.close()

	fi.close()	
	fo.close()


def doc_addr_and_name(line, docs_input_directory):
	a = line.split("\t")
	if(len(a) > 1):
		doc_addr = docs_input_directory + a[0].strip()
		doc_name = a[1].strip()
	else:
		print "Error"
		sys.exit()

	return doc_addr, doc_name	


if __name__ == "__main__":
	docs_input_directory = sys.argv[1]
	if(docs_input_directory[-1] != "/"):
		docs_input_directory += "/"

	docs_input_file = docs_input_directory + "docs.txt"

	embeddings_directory = sys.argv[2]
	if(embeddings_directory[-1] != "/"):
		embeddings_directory += "/"

	wordVec_input_file = embeddings_directory + "word.dat"
	output_file = embeddings_directory + "doc.dat"

	words, phi_words, K = readDocData.readWordsAndEmbeddings(wordVec_input_file)


	doc_embedding(docs_input_directory, docs_input_file, words, phi_words, K, output_file)


































