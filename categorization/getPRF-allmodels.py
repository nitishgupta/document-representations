import sys
from os import walk
import os.path
import prf_prediction

models = ["PMF", "wordvec-avg", "bow-tfidf", "weight-update-epoch10", "weight-update-epoch20", "weight-update-epoch50", "weight-update-epoch80", "weight-update-epoch100", 
		"weight-update-epoch120", "weight-update-epoch150", "weight-update-epoch150-K50", "weight-update-epoch150-K150", "weight-update-epoch150-lr001", "weight-update-epoch150-lr001-K50", 
		"weight-update-epoch150-lr001-K150", "lsi-tfidf-100", "lsi-tfidf-200"]

threshold = 0.5


evaluation = sys.argv[2]

dataset_output_directory  = sys.argv[1]
if dataset_output_directory[-1] != "/":
	dataset_output_directory +="/"

print "Printing Test Set P, R, F1 Scores : "
for model in models:
	filename = model+"-test.dat"
	file_address = dataset_output_directory+model+"/prediction/"+evaluation + "/" + filename
	if os.path.isfile(file_address):
		PRF1 = prf_prediction.getPRF(file_address, threshold)
		print model, "- P : ", PRF1[0], " R : ", PRF1[1], " F1 : ", PRF1[2]
	else:
		print model, "- PREDICTION FILE NOT FOUND!!!!";	
	
