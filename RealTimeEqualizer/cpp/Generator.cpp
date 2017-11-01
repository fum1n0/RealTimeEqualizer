#pragma once

#include"../hpp/Generator.hpp"


Generator::Generator() {
	
	generate.push_back([&]() { this->createWhiteNoise(); });
	generate.push_back([&]() { this->createSawtooth(); });

}


void Generator::createWhiteNoise() { // ƒzƒƒCƒgƒmƒCƒY
	
	std::random_device seed_gen;
	std::default_random_engine engine(seed_gen());
	std::uniform_real_distribution<> dist(-1.0, 1.0);
	for (auto& i : signal)i = dist(engine); // *e_rms;
	
}


void Generator::createSawtooth() { // ƒmƒRƒMƒŠ”g
	
	for (int i = 0; i < (int)signal.size();i++)
		signal[i] = 2 * (freq*i*time - floor(freq*i*time+0.5));	

}


void Generator::signalResize(size_t length) {
	signal = std::vector<double>(length, 0.0);
}

