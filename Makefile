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

epoch-test:
	./vocab-weight -train-directory $(TD) -output-directory $(OD)/weight-update-epoch10 -debug 0 -min-count 5 -epoch 10 -embed-size 100 -nthreads 30 -window-size 2 -negative-samples 10 -update-weights 1
	./vocab-weight -train-directory $(TD) -output-directory $(OD)/weight-update-epoch20 -debug 0 -min-count 5 -epoch 20 -embed-size 100 -nthreads 30 -window-size 2 -negative-samples 10 -update-weights 1
	./vocab-weight -train-directory $(TD) -output-directory $(OD)/weight-update-epoch50 -debug 0 -min-count 5 -epoch 50 -embed-size 100 -nthreads 30 -window-size 2 -negative-samples 10 -update-weights 1
	./vocab-weight -train-directory $(TD) -output-directory $(OD)/weight-update-epoch80 -debug 0 -min-count 5 -epoch 80 -embed-size 100 -nthreads 30 -window-size 2 -negative-samples 10 -update-weights 1
	./vocab-weight -train-directory $(TD) -output-directory $(OD)/weight-update-epoch100 -debug 0 -min-count 5 -epoch 100 -embed-size 100 -nthreads 30 -window-size 2 -negative-samples 10 -update-weights 1
	./vocab-weight -train-directory $(TD) -output-directory $(OD)/weight-update-epoch120 -debug 0 -min-count 5 -epoch 120 -embed-size 100 -nthreads 30 -window-size 2 -negative-samples 10 -update-weights 1
	./vocab-weight -train-directory $(TD) -output-directory $(OD)/weight-update-epoch150 -debug 0 -min-count 5 -epoch 150 -embed-size 100 -nthreads 30 -window-size 2 -negative-samples 10 -update-weights 1
