#pragma once

#include"../hpp/Vocoder.hpp"


void Vocoder::setExecute(bool flag) {
	doExecute = flag;
}

void Vocoder::setGenerateId(int id) {
	if (id < 0 || generate.size() <= id)return;
	generateId = id;
}


bool Vocoder::isExecute() {
	return doExecute;
}


Wave Vocoder::execute(Wave voice) {

	std::vector<double> tmp(voice.lengthSample);
	Wave wav(voice.lengthSample);
		
	for (size_t i = 0; i < tmp.size(); i++)tmp[i] = voice[i].left / 32768.0;

	if (*std::max_element(tmp.begin(), tmp.end()) < 0.0001)return voice;

	this->calc_formant(voice);
	signalResize(voice.lengthSample);
	generate[generateId]();

	for (size_t i = 0; i < tmp.size(); i++)tmp[i] = noise[i] * e_rms ;
	
	for (size_t i = 0; i<voice.lengthSample; i++) {
		for (size_t j = 1; j<a.size(); j++) {
			if (i >= j) tmp[i] -= a[j] * tmp[i - j];
		}
	}

	for (size_t i = 1; i < tmp.size(); i++)tmp[i] += tmp[i - 1] * 0.9;
	for (size_t i = 0; i < tmp.size(); i++)wav[i] = Waving::DoubleToSample(tmp[i]);

	return wav;
}
