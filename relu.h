#pragma once

#include "matrix.h"

class ReLU {
	matrix<double>* output;

public:
	void forward(matrix<double> input) {
		for (size_t i = 0; i < input.rows(); ++i) {
			for (size_t j = 0; j < input.columns(); ++j) {
				if (input(i, j) < 0.0)
					input(i, j) = 0.0;
			}
		}

		output = new matrix<double>(input);
	}

	matrix<double> getOutput() {
		matrix<double> out(*output);
		return out;
	}
};