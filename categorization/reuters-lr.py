from collections import defaultdict
import theano.tensor as T
from theano.tensor.shared_randomstreams import RandomStreams
import theano
import numpy as np
from sklearn.metrics import precision_recall_fscore_support as prf
from sklearn.metrics import accuracy_score
from random import shuffle
from sets import Set
import sys
import random as rng
from copy import deepcopy
import readDocData

def countOnes(lis):
	ones = 0
	zeros = 0;
	for l in lis:
		if (l == 1):
			ones = ones + 1
		else:
			zeros = zeros + 1	

	print "zeros : ", zeros, " ones", ones		

def split_TrainValTest(trainperc, valperc):
	shuffle(data);
	trlen = int(len(data)*trainperc)
	vallen = int(len(data)*valperc)
	trdata = data[0:trlen]
	vdata = data[trlen:trlen+vallen]
	testdata = data[trlen+vallen:-1]
	return trdata, vdata, testdata

def split_TrainVal(trainperc):
	shuffle(tr_v_data);
	trlen = int(len(tr_v_data)*trainperc)
	trdata = tr_v_data[0:trlen]
	vdata = tr_v_data[trlen:-1]
	return trdata, vdata

def split_TrainValTest_ColdStart(trainperc_docs, valperc, num_e1):
	shuffle(data)
	num_e1_test = int((1 - trainperc_docs + valperc)*num_e1)
	docs_test = rng.sample(range(0,num_e1), num_e1_test)
	size_val = int(len(data)*valperc)
	trdata = []
	vdata = []
	testdata = []
	for d in data:
		if(d[0] in docs_test):
			testdata.append(d)
		else:
			if(len(vdata) < size_val):
				vdata.append(d)
			else:
				trdata.append(d)

	return trdata, vdata, testdata			
			

def coldstart_sanity(trdata, vdata, testdata):
	test_docs = Set([])
	for d in testdata:
		test_docs.add(d[0])
	for d in trdata:
		if(d[0] in test_docs):
			print "Error in Cold Start Split", d[0], " found in training"
			sys.exit()

	for d in vdata:
		if(d[0] in test_docs):
			print "Error in Cold Start Split", d[0], " found in validation"
			sys.exit()

	print "Cold Start Split FINE!"		




# Add negative data using corrupt second entity. Therefore for every [e1, e2, truth], samples [e1, e2*, anti-truth] are added
def addNegativeData(in_data, num_e2, negative = 1):
	i = 0
	print "Negatives per data : ", negative
	in_data_size = len(in_data)
	for n in range(0, negative):
		for i in range(0, in_data_size):
			e1 = in_data[i][0]
			e2 = np.random.randint(low = 0, high = num_e2, size=1)
			in_data.append([e1, int(e2), 0])
		shuffle(in_data)	
	return in_data	


def getDataStats(dt):
	trues = 0
	false = 0
	for d in dt:
		if(d[2] == 1):
			trues = trues + 1
		else:
			false = false + 1

	print "true : ", trues, " false : ", false				

def getPRF(in_data, phi_d, phi_c):
	prediction = []			
	true = []
	for d in in_data:
		prediction.append(predict(d[0], d[1], phi_d, phi_c))
		true.append(d[2])

	PRF = prf(true, prediction, average = 'micro');	
	acc = accuracy_score(true, prediction)
	p = round(PRF[0]*100, 1);
	r = round(PRF[1]*100, 1);
	f1 = round(PRF[2]*100, 1);
	a = round(acc*100, 1);
	print "P : ", p, " R : ", r, " F1 : ", f1, " Acc = ", a
	return p, r, f1, a

def predict(e1, e2, phi_d, phi_c):
	pre_sigm = np.dot(phi_d[e1], phi_c[e2])
	sigm = 1 / (1 + np.exp(-pre_sigm))
	return sigm > 0.5

def getPRF_bias(in_data, phi_d, phi_c, bias_c):
	prediction = []			
	true = []
	for d in in_data:
		prediction.append(predict_bias(d[0], d[1], phi_d, phi_c, bias_c))
		true.append(d[2])

	PRF = prf(true, prediction, average = 'micro');	
	acc = accuracy_score(true, prediction)
	p = round(PRF[0]*100, 1);
	r = round(PRF[1]*100, 1);
	f1 = round(PRF[2]*100, 1);
	a = round(acc*100, 1);
	print "P : ", p, " R : ", r, " F1 : ", f1, " Acc = ", a
	return p, r, f1, a	

def predict_bias(e1, e2, phi_d, phi_c, bias_c):
	pre_sigm = np.dot(phi_d[e1], phi_c[e2]) + bias_c[e2]
	sigm = 1 / (1 + np.exp(-pre_sigm))
	return sigm > 0.5


def update(e1, e2, truth, b, lr, reg_con):
	pre_sigm = np.dot(phi_docs[e1], phi_cats[e2])
	if(b):
		pre_sigm += bias_cats[e2]
	sigm = 1 / (1 + np.exp(-pre_sigm))
	
	#phi_docs[e1] = phi_docs[e1] + lr*( (truth - sigm)*phi_cats[e2] - reg_con*phi_docs[e1] )
	phi_cats[e2] = phi_cats[e2] + lr*( (truth - sigm)*phi_docs[e1] - reg_con*phi_cats[e2] )
	if(b):
		bias_cats[e2] = bias_cats[e2] + lr*(truth - sigm - reg_con*bias_cats[e2])

	return sigm

def mf_train(b, lr, lamb, epoch, neg):
	best_f1 = 0.0
	best_epoch = 0;
	for i in range(0, epoch):
		shuffle(trdata)
		for d in trdata:
			update(d[0], d[1], d[2], b, lr, lamb)		## Positive Sample e1, e2
			for n in range(0, neg):
				neg_cat = np.random.randint(low=0, high=num_e2)
				neg_cat = int(neg_cat)
				update(d[0], neg_cat, 0, b, lr, lamb)

		if(i % 5 == 0):
			print "Epoch : ", i, ", ",
			if(b):
				p, r, f1, acc = getPRF_bias(vdata, phi_docs, phi_cats, bias_cats)
			else:
				p, r, f1, acc = getPRF(vdata, phi_docs, phi_cats)	
			if(f1 >= best_f1):
				phi_cats_best = np.copy(phi_cats)
				phi_docs_best = np.copy(phi_docs)
				best_epoch = i
				best_f1 = f1
	print "Best Epoch : ", best_epoch			
	print "Test : ",
	if(b):
		p_t, r_t, f1_t, acc_t = getPRF_bias(testdata, phi_docs_best, phi_cats_best, bias_cats)		
	else:	
		p_t, r_t, f1_t, acc_t = getPRF(testdata, phi_docs_best, phi_cats_best)		
	
	return phi_docs_best, phi_cats_best			


def write_predictions(phi_best_docs, phi_best_cats, prediction_filename):
	out_val_file = open(prediction_filename + "-val.dat", 'w')
	out_test_file = open(prediction_filename + "-test.dat", 'w')

	for d in vdata:
		predict_pre_sigm_activation = np.dot(phi_best_docs[d[0]], phi_best_cats[d[1]])
		sigm = 1 / (1 + np.exp(-predict_pre_sigm_activation))
		truth = d[2]
		out_val_file.write(str(sigm) + "\t" + str(truth) + "\n")

	for d in testdata:
		predict_pre_sigm_activation = np.dot(phi_best_docs[d[0]], phi_best_cats[d[1]])
		sigm = 1 / (1 + np.exp(-predict_pre_sigm_activation))
		truth = d[2]
		out_test_file.write(str(sigm) + "\t" + str(truth) + "\n")

def write_cat_embeddings(phi_c, cats, K, out_file_path):
	fo = open(out_file_path, 'w')
	num_cats = len(cats.keys())
	fo.write(str(num_cats) + "\t" + str(K) + "\n")

	for cat in cats.keys():
		cat_id = cats[cat]
		fo.write(cat)
		for i in range(0, K):
			fo.write("\t" + str(phi_c[cat_id][i]))
		fo.write("\n")	
	fo.close()			





if __name__=="__main__":
	rng.seed(20)
	np.random.seed(10)
	
	dataset_directory = sys.argv[1]
	if(dataset_directory[-1] != "/"):
			dataset_directory += "/"

	datafilename = dataset_directory + "doc_category.txt"
	train_id_file = dataset_directory + "train_id.txt"
	test_id_file = dataset_directory + "test_id.txt"
	
	dataset_output_directory = sys.argv[2]
	if(dataset_output_directory[-1] != "/"):
		dataset_output_directory += "/"

	model = sys.argv[3]

	phi_docs_file = dataset_output_directory+model+"/embeddings/doc.dat"
	prediction_out_file = dataset_output_directory+model+"/prediction/"+"cold-start"+"/"+ model
	cat_embed_outfile = dataset_output_directory+model+"/embeddings/"+"category.dat"

	K = 100
	train_perc = 0.8
	val_perc = 0.1
	negative_data = 1
	learning_rate = 0.01
	reg_con = 0.01
	epoch = 600
	negative_training = 1
	bias = 0

	if(len(sys.argv) == 6):
		if(sys.argv[5] == "bias"):
			print "Using bias"
			bias = 1
		else:	
			epoch = int(sys.argv[5])
	
	docs, cats, catC, tr_v_data, testdata = readDocData.read_Reuters(datafilename, train_id_file, test_id_file);
	num_e1 = len(docs.keys())
	num_e2 = len(cats.keys())
	print "d : ", num_e1, " c : ", num_e2
	
	phi_docs, K = readDocData.read_Phi_Docs(phi_docs_file, docs)
	phi_cats = np.random.normal(loc=0.0, scale=0.01, size=(num_e2, K))
	bias_cats = np.zeros(shape=num_e2)

	#trdata, vdata, testdata = split_TrainValTest(train_perc, val_perc);
	trdata, vdata = split_TrainVal(0.9)

	
	print len(tr_v_data)+len(testdata), len(trdata), len(vdata), len(testdata)
	

	testdata = addNegativeData(testdata, num_e2, negative = negative_data)
	vdata = addNegativeData(vdata, num_e2, negative = negative_data)

	getDataStats(trdata)
	getDataStats(vdata)
	getDataStats(testdata)
	

	phi_best_docs, phi_best_cats = mf_train(b= bias, lr= learning_rate, lamb= reg_con, epoch= epoch, neg = negative_training)
	write_predictions(phi_best_docs, phi_best_cats, prediction_out_file)
	write_cat_embeddings(phi_best_cats, cats, K, cat_embed_outfile)
	
