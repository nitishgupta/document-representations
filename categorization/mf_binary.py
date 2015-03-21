from collections import defaultdict
import theano.tensor as T
from theano.tensor.shared_randomstreams import RandomStreams
import theano
import numpy as np
from sklearn.metrics import precision_recall_fscore_support as prf
from random import shuffle
import sys
import random as rng
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

def getDataStats(dt):
	trues = 0
	false = 0
	for d in dt:
		if(d[2] == 1):
			trues = trues + 1
		else:
			false = false + 1

	print "true : ", trues, " false : ", false				

def update(e1, e2, truth, lr, reg_con):
	pre_sigm = np.dot(phi_docs[e1], phi_cats[e2])
	sigm = 1 / (1 + np.exp(-pre_sigm))
	phi_docs[e1] = phi_docs[e1] + lr*( (truth - sigm)*phi_cats[e2] - reg_con*phi_docs[e1] )
	phi_cats[e2] = phi_cats[e2] + lr*( (truth - sigm)*phi_docs[e1] - reg_con*phi_cats[e2] )
	return sigm

def predict(e1, e2):
	pre_sigm = np.dot(phi_docs[e1], phi_cats[e2])
	sigm = 1 / (1 + np.exp(-pre_sigm))
	return sigm > 0.5

def mf_train(lr, lamb, epoch):
	for i in range(0, epoch):
		shuffle(trdata)
		for d in trdata:
			update(d[0], d[1], d[2], lr, lamb)		## Positive Sample e1, e2

		if(i % 5 == 0):
			val_prediction = []			
			val_true = []
			for d in vdata:
				val_prediction.append(predict(d[0], d[1]))
				val_true.append(d[2])

			PRF = prf(val_true, val_prediction, average = 'micro');	
			p = round(PRF[0]*100, 1);
			r = round(PRF[1]*100, 1);
			f = round(PRF[2]*100, 1);
			print "Epoch : ", i, "P : ", p, " R : ", r, " F1 : ", f	


	test_prediction = []			
	test_true = []
	for d in testdata:
		test_prediction.append(predict(d[0], d[1]))
		test_true.append(d[2])

	PRF = prf(test_true, test_prediction, average = 'micro');	
	p = round(PRF[0]*100, 1);
	r = round(PRF[1]*100, 1);
	f = round(PRF[2]*100, 1);
	print "Test : ", "P : ", p, " R : ", r, " F1 : ", f		
			

if __name__=="__main__":
	datafilename = sys.argv[1]
	#docs, cats, data = readDoc_Cat_Data.read(datafilename);
	buss, atts, data = readDocData.readYelp(datafilename);
	#buss, atts, data = readDoc_Cat_Data.readAmazon(datafilename);
	num_bus = len(buss.keys())
	num_atts = len(atts.keys())
	print "b : ", num_bus, " a : ", num_atts
	phi_docs = a = np.random.normal(loc=0.0, scale=0.01, size=(num_bus,30))
	phi_cats = a = np.random.normal(loc=0.0, scale=0.01, size=(num_atts,30))

	#docs, cats, data = readDoc_Cat_Data.readAmazon(datafilename);

	trdata, vdata, testdata = split_TrainValTest(0.8, 0.1);
	print len(data), len(trdata), len(vdata), len(testdata)

	getDataStats(trdata)
	getDataStats(vdata)
	getDataStats(testdata)
	

	mf_train(lr=0.01, lamb=0.001, epoch=100)
	