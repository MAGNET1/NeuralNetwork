#pragma once

#include "matrix.h"
#include <math.h>
#include <numeric>

class Softmax {
	matrix<double>* output;

public:
	void forward(matrix<double> input) {
		matrix<double> exp_values = input - input.max(1, true);
		exp_values.exp();

		exp_values /= exp_values.sum(1, true);

		output = new matrix<double>(exp_values);
	}

	matrix<double> getOutput() {
		matrix<double> out(*output);
		return out;
	}
};