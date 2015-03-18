import preprocess
from os import walk
import sys
import os

directory = sys.argv[1]
if(directory[-1] != "/"):
	directory = directory + "/"

files = []

for (dirpath, dirnames, filenames) in walk(directory):
		files = filenames
		break


for filename in files:
	fi = open(directory + filename, 'r')		
	text = preprocess.readInputText(fi)
	fi.close();
	fo = open(directory + filename, 'w')
	preprocess.writeOutput(text, fo)
	fo.close()

print "Done!"	


























