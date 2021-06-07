#pragma once

#include "matrix.h"

class Dense {
	matrix<double>* weights;
	matrix<double>* biases;
	matrix<double>* output;
public:

	Dense(size_t n_inputs, size_t n_neurons) {
		weights = new matrix<double>(n_inputs, n_neurons);
		biases = new matrix<double>(1, n_neurons);
		weights->randomizeDouble();
		biases->randomizeDouble();
		output = nullptr;
	}

	void forward(matrix<double> input) {
		// matrix<double> out = input * (*weights) + (*biases);
		output = new matrix<double>(input * (*weights) + (*biases));
	}

	void showWeights() {
		std::cout << *weights;
	}

	void showBiases() {
		std::cout << *weights;
	}

	matrix<double> getOutput() {
		matrix<double> out(*output);
		return out;
	}
};