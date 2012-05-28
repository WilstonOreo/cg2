#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "cg2/Median.hpp"
#include <algorithm>

#define NUM_ELEMENTS 101

using namespace std;

int main() {
	vector<double> A(NUM_ELEMENTS);

	srand(time(NULL));
	for (int i = 0; i < NUM_ELEMENTS; i++) {
		A[i] = (double)rand()/RAND_MAX;
	}
	cout << "Elements of array A: " << endl;

	for (int i = 0; i < NUM_ELEMENTS; i++) {
		cout << i << " = " << A[i] << ", ";
	}
	cout << endl;


	double a = median(A);
	cout << "Median of A is: " << a << endl;

	//cout << "

	sort(A.begin(),A.end());
	cout << "Elements of array A: " << endl;

	for (int i = 0; i < NUM_ELEMENTS; i++) {
		cout << i << " = " << A[i] << ", ";
	}
	cout << endl;


	if (A[A.size()/2] == a) {
		cout << "Everything is fine." << endl;
		return EXIT_SUCCESS;
	}
	else {
		cout << "Results mismatch: " << a << " vs. " << A[(A.size()-1)/2] << endl;
		return EXIT_FAILURE;
	}
}
