#pragma once

#include"../hpp/Generator.hpp"


void Generator::createWhiteNoise(double sample) { // ƒzƒƒCƒgƒmƒCƒY
	
	std::random_device seed_gen;
	std::default_random_engine engine(seed_gen());
	std::uniform_real_distribution<> dist(-1.0, 1.0);

	signal = std::vector<double>(sample, 0.0);
	for (auto& i : signal)i = dist(engine); // *e_rms;
	
}


void Generator::createSawtooth(double sample) { // ƒmƒRƒMƒŠ”g

	signal = std::vector<double>(sample, 0.0);
	
	for (int i = 0; i < (int)sample;i++)
		signal[i] = 2 * (freq*i*time - floor(freq*i*time+0.5));	

}



