import sys
import os

ourmodel = "ourmodel-nw"
indir = "output/Combined/" + ourmodel + "/prediction/"
ev = "held-out"
testval = 'test'
outfile = open(indir+ev+ "/"+ ourmodel + "-" +testval+".dat", 'w')

for root, dirs, files in os.walk(indir):
    for filename in files:
    	if(filename.split("-")[1].strip() == "test.dat"):
    		print filename
    		f = open(indir+filename, 'r')
    		print indir+filename
    		t = f.read()
    		outfile.write(t)
    		f.close()

outfile.close()