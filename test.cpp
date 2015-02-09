#include <limits>
#include <stdio.h>
#include <iostream>

int main(){
	int imin = std::numeric_limits<unsigned int>::min(); // minimum value
	int imax = std::numeric_limits<unsigned int>::max();
	std::cout <<imin <<" " <<imax;
}

