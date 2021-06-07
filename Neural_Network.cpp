#include <iostream>
#include <assert.h>
#include "dense.h"
#include "relu.h"
#include "softmax.h"

int main() {

	srand(1331123);

	matrix<double> input(10, 5);
	input.randomizeDouble();

	Dense layer1(5, 5);
	layer1.forward(input);

	ReLU relu1;
	relu1.forward(layer1.getOutput());

	 Dense layer2(5,3);
	 layer2.forward(relu1.getOutput());

	 ReLU relu2;
	 relu2.forward(layer2.getOutput());

	 Dense layer3(3,7);
	 layer3.forward(relu2.getOutput());

	 ReLU relu3;
	 relu3.forward(layer3.getOutput());

	 Softmax soft;
	 soft.forward(relu3.getOutput());
	 matrix<double> out = soft.getOutput();
	 std::cout << out;
}