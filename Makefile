CC = g++
#Using -Ofast instead of -O3 might result in faster code, but is supported only by newer GCC version
CXXFLAGS = -std=c++11
all: 
	$(CC) vocab.cpp -o vocab $(CXXFLAGS) 

t:
	$(CC) test.cpp -o test $(CXXFLAGS)	

run:
	./vocab -train-directory data -debug 0 -min-count 1

debug:
	./vocab -train-directory data/ -debug 1 -min-count 1
