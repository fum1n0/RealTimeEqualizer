#pragma once

#include"Public.hpp"

class Generator {


private:

	const double time = 1.0 / 44100.0;
	std::vector<double> signal;
	double freq;

public:

	void createWhiteNoise(double);
	void createSawtooth(double);

};