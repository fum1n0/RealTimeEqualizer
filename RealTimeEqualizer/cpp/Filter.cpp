#pragma once

#include"../hpp/Filter.hpp"


Filter::Filter(int size, int dimension) {

	signal = std::vector<double>(2*size, 0.0);
	filter = std::vector<double>(2 * dimension + 1, 0.0);
	recordplay = std::make_shared<RecordPlay>();
}


void Filter::update() {
	recordplay->update();
}


void Filter::change(std::vector<double> X, int dimension) {

	freq = X;
	degree = 2 * dimension + 1;
	filter = std::vector<double>(degree, 0.0);
	signal = ifft_execute();

	for (int i = 0; i < filter.size(); i++)	filter[i] = signal[((int)filter.size() - dimension + i) % (int)filter.size()];
	
	recordplay->setFilter(filter);
}


std::vector<double> Filter::ifft_execute() {


	std::vector<double>re(2 * freq.size(), 0.0);
	std::vector<double>im(2 * freq.size(), 0.0);

	std::vector<double>inv = freq;
	std::reverse(inv.begin(), inv.end());

	for (int i = 0; i < freq.size(); i++) {
		re[i] = freq[i];
		re[freq.size() + i] = inv[i];
	}

	double w;
	std::vector<double>wr;
	std::vector<double>wi;

	w = -1.0 * 2.0 * Pi / (double)re.size();
	wr = std::vector<double>((int)re.size(), 0);
	wi = std::vector<double>((int)re.size(), 0);
	for (int i = 0; i < (int)re.size(); i++) {
		wr[i] = cos(w * i);
		wi[i] = -sin(w*i);
	}


	long i = 0;


	for (int j = 1; j < (int)re.size() - 1; j++) {
		for (int k = (int)re.size() >> 1; k > (i ^= k); k >>= 1); // k=2^{n-1}; k > i = i xor k; k/2
		if (j < i) {
			std::swap(re[i], re[j]);
			std::swap(im[i], im[j]);
		}
	}



	double xr1, xr2, xi1, xi2;
	int s, t, turn1, turn2, turn3;

	for (int rep = 2; rep <= (int)re.size(); rep *= 2) { // 一番外側のrep, 2DFT -> 4DFT -> ... -> NDFT
		s = (int)re.size() / rep; // ブロック数
		t = rep / 2; // 1ブロックが保有する数の半分
		for (int u = 0; u < s; u++) { // NDFTの中のブロック数
			for (int v = 0; v < t; v++) { // ブロックの半分まで

				turn1 = v + rep*u;
				turn2 = s*v;
				turn3 = s*(v + t);

				xr1 = re[turn1];
				xr2 = re[turn1 + t];
				xi1 = im[turn1];
				xi2 = im[turn1 + t];


				re[turn1] = xr1 + xr2 * wr[turn2] - xi2 * wi[turn2];
				im[turn1] = xi1 + xi2 * wr[turn2] + xr2 * wi[turn2];

				re[turn1 + t] = xr1 + xr2 * wr[turn3] - xi2 * wi[turn3];
				im[turn1 + t] = xi1 + xi2 * wr[turn3] + xr2 * wi[turn3];


			}
		}
	}

	for (int k = 0; k < (int)re.size(); k++) {
		re[k] /= (double)re.size();
		im[k] /= (double)im.size();
	}

	return re;
}


