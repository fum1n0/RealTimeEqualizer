#pragma once

#include"Public.hpp"

class Generator {


protected:

	const double time = 1.0 / 44100.0;
	int freq;
	std::vector<double> noise;
	std::vector<std::function<void()>>generate;

public:

	Generator();
	void createWhiteNoise();
	void createSawtooth();
	void signalResize(size_t);
	void setFreq(int);

};