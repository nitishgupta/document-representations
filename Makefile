CC = g++
#Using -Ofast instead of -O3 might result in faster code, but is supported only by newer GCC version
CXXFLAGS = -std=c++11
all: 
	$(CC) vocab.cpp -o vocab $(CXXFLAGS) 
	
