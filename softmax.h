#pragma once

#include "matrix.h"
#include <math.h>
#include <numeric>

class Softmax {
	matrix<double>* output;

public:
	void forward(matrix<double> input) {
		output = new matrix<double>(input);

		*output -= input.max(1, true);
		output->exp();

		*output /= output->sum(1, true);
	}

	matrix<double> getOutput() {
		matrix<double> out(*output);
		return out;
	}
};