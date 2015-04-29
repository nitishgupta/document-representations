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

models = ['PMF', 'bow-tfidf', 'lsi-tfidf-100', 'wordvec-avg', 'ourmodel-nw', 'ourmodel-ww']
#models = ["PMF"]
legend = {}

for model in models:
	print model
	if(model == "PMF"):
		legend[model] = "PMF"
		continue

	if(model.split("-")[0] == "weight" and model.split("-")[1] == "update"):
		legend[model] = "Our Model (with weights)"

	if(model.split("-")[0] == "weight" and model.split("-")[1] == "noupdate"):
		legend[model] = "Our Model (no weights)"

	if(model.split("-")[0] == "lsi"):
		legend[model] = "LSI-100"			

	if(model.split("-")[0] == "bow"):
		legend[model] = "BOW"			

	if(model.split("-")[0] == "wordvec"):
		legend[model] = "WordVecAvg"

	if(model.split("-")[1] == "ww"):
		legend[model] = "Our Model (with weights)"

	if(model.split("-")[1] == "nw"):
		legend[model] = "Our Model (no weights)"

			


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

fig = plt.figure()
ax = plt.subplot(111)
output_file = output_directory+"Test-" + evaluation + "-PRCurve.pdf"
for model in models:
	prediction_file = output_directory+model+"/prediction/"+evaluation+"/"+model+"-test.dat"
	y_true, y_pred = read_prediction_data(prediction_file)
	print len(y_true), len(y_pred)
	p, r, thresholds = precision_recall_curve(y_true, y_pred)
	print len(p), len(r), len(thresholds)
	#plot_prcurve(model, p, r, output_file)
	#h = plt.plot(r, p, label=str(model))

	ax.plot(r, p, label=legend[model])
	area = auc(r, p)
	print model, " : ", area

	
#plt.legend(hans, loc=1)	
ax.legend(loc='lower left')
plt.xlabel('Recall')
plt.ylabel('Precision')
plt.xlim([0.0, 1.0])
plt.savefig(output_file)