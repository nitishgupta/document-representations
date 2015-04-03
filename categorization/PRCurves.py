import sys
import numpy as np
import pylab as pl
from sklearn.metrics import precision_recall_curve
from sklearn.metrics import auc
import matplotlib.pyplot as plt

output_directory = sys.argv[1]
if(output_directory[-1] != "/"):
	output_directory += "/"

evaluation = sys.argv[2]

models = ['PMF', 'wordvec-avg', 'bow-tfidf', 'lsi-tfidf-100', 'weight-update-epoch150', 'weight-update-epoch150-K50']
#models = ["PMF"]


def read_prediction_data(prediction_file):
	fi = open(prediction_file, 'r')
	y_pred = []
	y_true = []
	for line in fi:
		line.strip();
		a = line.split("\t")
		if(len(a) == 2):
			y_pred.append(float(a[0].strip()));
			y_true.append(float(a[1].strip()))

	return y_true, y_pred		

def plot_prcurve(model, precision, recall, output_file):
	# pl.clf()
	# pl.plot(recall, precision, label=model)
	# pl.xlabel('Recall')
	# pl.ylabel('Precision')
	# pl.ylim([0.0, 1.05])
	# pl.xlim([0.0, 1.0])
	# pl.title('Precision-Recall Curve')
	# pl.legend(loc="top right")
	# pl.show()
	plt.plot(recall, precision)
	plt.savefig(output_file)
	#plt.show()

hans=[]
output_file = output_directory+"Test-" + evaluation + "-PRCurve.pdf"
for model in models:
	prediction_file = output_directory+model+"/prediction/"+evaluation+"/"+model+"-test.dat"
	y_true, y_pred = read_prediction_data(prediction_file)
	p, r, thresholds = precision_recall_curve(y_true, y_pred)
	#plot_prcurve(model, p, r, output_file)
	h = plt.plot(r, p, label=str(model))
	area = auc(r, p)
	print model, " : ", area

	hans.append(h)
	
#plt.legend(hans, loc=1)	
plt.savefig(output_file)