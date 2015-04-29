import sys
import os

datasets = ['Physics', 'Biology', 'Mathematics', 'Sports']
evaluation = 'held-out'

model = 'wordvec-avg'
testval =  'test'

write_directory = "output/Combined/"
outfile = write_directory + model + "/prediction/held-out/" + model+"-test.dat"
fo = open(outfile, 'w')

for dataset in datasets:
	print "output/"+dataset+"/"+model+"/prediction/held-out"+model+"-test.dat"
	fi = open("output/"+dataset+"/"+model+"/prediction/held-out/"+model+"-test.dat", 'r')
	t = fi.read()
	fo.write(t)
	fi.close()

fo.close()	



