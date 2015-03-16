CC = g++
#+Using -Ofast instead of -O3 might result in faster code, but is supported only by newer GCC version
CXXFLAGS = -lm -pthread -std=c++11
CFLAGS = -lm -pthread
all: 
#	$(CC) vocab.cpp -o vocab $(CXXFLAGS) 
	$(CC) -c vocab.cpp -o vocab.o $(CXXFLAGS)
	$(CC) -c util.cpp -o util.o $(CXXFLAGS)
	$(CC) vocab.o util.o -o vocab $(CXXFLAGS)
	rm vocab.o util.o

t:
	$(CC) test.cpp -o test $(CXXFLAGS)	

run:
	./vocab -train-directory data -debug 0 -min-count 0

sample:
	./vocab -train-directory sample_data/ -output-directory output/sample_data/ -debug 0 -min-count 1 -sample -1

debug:
	./vocab -train-directory data/ -debug 1 -min-count 1
