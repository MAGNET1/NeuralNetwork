#pragma once

#include "matrix.h"
#include <math.h>
#include <numeric>

class Softmax {
	matrix<double>* output;

public:
	void forward(matrix<double> input) {
		for (size_t i = 0; i < input.rows(); ++i) {
			for (size_t j = 0; j < input.columns(); ++j) {
				input(i, j) = exp(input(i, j));
			}
		}

		double sum = input.sum();

		for (size_t i = 0; i < input.rows(); ++i) {
			for (size_t j = 0; j < input.columns(); ++j) {
				input(i, j) = input(i, j) / sum;
			}
		}

		output = new matrix<double>(input);
	}

	matrix<double> getOutput() {
		matrix<double> out(*output);
		return out;
	}
};