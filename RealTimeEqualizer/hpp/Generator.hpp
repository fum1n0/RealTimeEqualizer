#pragma once

#include"Public.hpp"

class Generator {


private:

	const double time = 1.0 / 44100.0;
	std::vector<double> signal;
	double freq;
	std::vector<std::function<void()>>generate;

public:

	Generator();
	void createWhiteNoise();
	void createSawtooth();
	void signalResize(size_t);
};