import sys
from sets import Set


def name_doc_category(doc_list_filename, doc_cat_outfile, docs_outfile):
	fo_dc = open(doc_cat_outfile, 'w')
	fo_docs = open(docs_outfile, 'w')
	fi = open(doc_list_filename, 'r')

	for line in fi:
		name = line.split(" ", 1)
		categories = name[1].strip()
		doc_id = name[0].strip().split("/")[1].strip()
		fo_dc.write(doc_id)
		fo_docs.write(name[0].strip() + "\t" + doc_id + "\n")
		cats = categories.split(" ")
		for cat in cats:
			if(len(cat.strip()) >= 1):
				fo_dc.write("\t" + cat.strip())

		fo_dc.write("\n")		

	fi.close()
	fo_dc.close()
	fo_docs.close()


def create_id_files(split_file, train_out, test_out):
	fi = open(split_file, 'r')
	fo_train = open(train_out, 'w')
	fo_test = open(test_out, 'w')

	for line in fi:
		name = line.split(" ", 1)
		tt = name[0].strip().split("/")[0].strip()
		doc_id = name[0].strip().split("/")[1].strip()
		if(tt == "training"):
			fo_train.write(doc_id + "\n")
		elif(tt == "test"):
			fo_test.write(doc_id + "\n")

	
	fi.close()
	fo_train.close()
	fo_test.close()	
				
		


if __name__=="__main__":
	outdirectoy = sys.argv[1]
	if(outdirectoy[-1] != "/"):
		outdirectoy += "/"
	doccat_list_filename = outdirectoy + "cats.txt"
	doc_cat_outfile = outdirectoy + "doc_category.txt"
	docs_outfile = outdirectoy + "docs.txt"
	#name_doc_category(doccat_list_filename, doc_cat_outfile, docs_outfile)

	train_out = docs_outfile = outdirectoy + "train_id.txt"
	test_out = docs_outfile = outdirectoy + "test_id.txt"
	create_id_files(doccat_list_filename, train_out, test_out)
