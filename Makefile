CC = g++
#+Using -Ofast instead of -O3 might result in faster code, but is supported only by newer GCC version
CXXFLAGS = -lm -pthread -std=c++11
CFLAGS = -lm -pthread
all: 
#	$(CC) vocab.cpp -o vocab $(CXXFLAGS) 
	$(CC) -c vocab-matrix.cpp -o vocab-matrix.o $(CXXFLAGS)
	$(CC) -c util.cpp -o util.o $(CXXFLAGS)
	$(CC) vocab-matrix.o util.o -o vocab-matrix $(CXXFLAGS)
	rm vocab-matrix.o util.o

	$(CC) -c vocab-weight.cpp -o vocab-weight.o $(CXXFLAGS)
	$(CC) -c util.cpp -o util.o $(CXXFLAGS)
	$(CC) vocab-weight.o util.o -o vocab-weight $(CXXFLAGS)
	rm vocab-weight.o util.o

t:
	$(CC) test.cpp -o test $(CXXFLAGS)	

run:
	./vocab -train-directory data -debug 0 -min-count 0

sample:
	./vocab -train-directory sample_data/ -output-directory output/sample_data/ -debug 0 -min-count 1 -sample -1

debug:
	./vocab -train-directory data/ -debug 1 -min-count 1
