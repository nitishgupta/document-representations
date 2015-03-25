## Call getPRF(fileName) to get [P, R, F]

import numpy as np;
from sklearn.metrics import precision_recall_fscore_support as prf
import sys

def readFile(fileName, threshold):
	y_pred = []
	y_true = []
	y_pred_true = []
	f = open(fileName, 'r');
	for line in f:
		line.strip();
		a = line.split("\t")
		if(float(a[0].strip()) >= threshold):
			pred = 1;
			y_pred.append(pred);
		else:
			pred = 0;
			y_pred.append(pred);
		
		true = float(a[1].strip());
		y_true.append(true);		
	y_pred_true.append(y_pred)
	y_pred_true.append(y_true)
	return y_pred_true

def getPRF(fileName, threshold):
	y_p_t = readFile(fileName, threshold)		
	y_pred = y_p_t[0];
	y_true = y_p_t[1];
	acc = prf(y_true, y_pred, average = 'micro');
	p = round(acc[0]*100, 1);
	r = round(acc[1]*100, 1);
	f = round(acc[2]*100, 1);
	return np.array([p,r,f])

if __name__ == "__main__":
	val_input_filename = sys.argv[1]
	test_input_filename = sys.argv[2]

	threshold = 0.1
	while (threshold < 1.0):
		print "Threshold : ", threshold

		PRF1 = getPRF(val_input_filename, threshold)
		print "Validation P : ", PRF1[0], " R : ", PRF1[1], " F1 : ", PRF1[2]

		PRF1 = getPRF(test_input_filename, threshold)
		print "Test P : ", PRF1[0], " R : ", PRF1[1], " F1 : ", PRF1[2]
		print "\n"

		threshold += 0.1




#fileName = "A-A"
#print getPRF(fileName)
