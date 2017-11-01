#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS

#include"../hpp/LPC.hpp"





void LPC::setLPC(Wave generate) {
	
	wav_zero = Wave(1); // 0-pound
	wav_base = generate;
	Frame_L = (int)pow(2, floor(log2(generate.lengthSample)) + 1);
	
	length = wav_base.lengthSample;
	
	wav_zero[0] = Waving::DoubleToSample(0.0);

	for (size_t m = generate.lengthSample; m < Frame_L; m++)wav_base.push_back(wav_zero[0]);

	wav_pre = wav_base;
	wav_ana = Wave(Frame_L);

	// 高音強調
	for (int m = 1; m < length; m++)
		wav_pre[m] = Waving::DoubleToSample(((double)wav_base[m].left / 32768.0) - ((double)wav_base[m - 1].left / 32768.0) * pre_emphasis);
	
}



void LPC::calc_formant(Wave record) {
	setLPC(record);
	init();
	hanning_execute();
	calc_ACF_FFT();
	calc_Levinson_Durbin();
	calc_error();
}


void LPC::init() {
	signal = std::vector<double>(Frame_L, 0);
	y = std::vector<double>();
	re = std::vector<double>(Frame_L, 0);
	im = std::vector<double>(Frame_L, 0);
	amp = std::vector<double>(Frame_L, 0);
	power = std::vector<double>(Frame_L, 0);
	dbv = std::vector<double>(Frame_L, 0);
	
	a = std::vector<double>(Order + 1, 0);
	b = std::vector<double>(Order + 1, 0);
	
	r = std::vector<double>(Frame_L, 0);
	e = std::vector<double>(Frame_L, 0);
	
	e_rms = 0.0;
}

void LPC::hanning_execute() {

	for (int k = 0; k < Frame_L; k++) {
		wav_ana[k] = Waving::DoubleToSample(((double)wav_pre[k].left / 32768.0) * (0.5 - 0.5*cos(2 * Pi*k / Frame_L)));
		signal[k] = wav_ana[k].left / 32768.0; // ハニング補正,正規化
	}
}


void LPC::fft_excute(std::vector<double>& signal_ptr, std::vector<double>& re_ptr, std::vector<double>& im_ptr, int inv) {

	std::vector<double>sig = signal_ptr;

	double w;
	std::vector<double>wr;
	std::vector<double>wi;
	w = (double)inv * 2.0 * Pi / (double)sig.size();
	wr = std::vector<double>((int)sig.size(), 0);
	wi = std::vector<double>((int)sig.size(), 0);
	for (int i = 0; i < (int)sig.size(); i++) {
		wr[i] = cos(w * i);
		wi[i] = -sin(w*i);
	}


	long i = 0;

	if (inv > 0) {
		for (int j = 1; j < (int)sig.size() - 1; j++) {
			for (int k = (int)sig.size() >> 1; k > (i ^= k); k >>= 1); // k=2^{n-1}; k > i = i xor k; k/2
			if (j < i) {
				std::swap(sig[i], sig[j]);
			}
		}
		re_ptr = sig;

	}
	else {
		for (int j = 1; j < (int)sig.size() - 1; j++) {
			for (int k = (int)sig.size() >> 1; k > (i ^= k); k >>= 1); // k=2^{n-1}; k > i = i xor k; k/2
			if (j < i) {
				std::swap(re_ptr[i], re_ptr[j]);
				std::swap(im_ptr[i], im_ptr[j]);
			}
		}
	}


	double xr1, xr2, xi1, xi2;
	int s, t, turn1, turn2, turn3;

	for (int rep = 2; rep <= (int)sig.size(); rep *= 2) { // 一番外側のrep, 2DFT -> 4DFT -> ... -> NDFT
		s = (int)sig.size() / rep; // ブロック数
		t = rep / 2; // 1ブロックが保有する数の半分
		for (int u = 0; u < s; u++) { // NDFTの中のブロック数
			for (int v = 0; v < t; v++) { // ブロックの半分まで

				turn1 = v + rep*u;
				turn2 = s*v;
				turn3 = s*(v + t);

				xr1 = re_ptr[turn1];
				xr2 = re_ptr[turn1 + t];
				xi1 = im_ptr[turn1];
				xi2 = im_ptr[turn1 + t];


				re_ptr[turn1] = xr1 + xr2 * wr[turn2] - xi2 * wi[turn2];
				im_ptr[turn1] = xi1 + xi2 * wr[turn2] + xr2 * wi[turn2];

				re_ptr[turn1 + t] = xr1 + xr2 * wr[turn3] - xi2 * wi[turn3];
				im_ptr[turn1 + t] = xi1 + xi2 * wr[turn3] + xr2 * wi[turn3];


			}
		}
	}

	if (inv > 0) { // フーリエ変換
		for (int k = 0; k < (int)re_ptr.size(); k++) {
			re_ptr[k] /= (double)re_ptr.size();
			im_ptr[k] /= (double)im_ptr.size();
		}
	}
	else signal_ptr = re_ptr; // 逆フーリエ変換


}


void LPC::calc_ACF_FFT() {

	std::vector<double> sig = signal;

	fft_excute(sig, re, im, 1); // フーリエ変換

	
	for (int i = 0; i < Frame_L; i++) {
		amp[i] = sqrt(re[i] * re[i] + im[i] * im[i]);
		dbv[i] = 20 * log10(amp[i]);	
	}
	

	for (int i = 0; i < Frame_L; i++)sig.push_back(0.0); // 0-padding

	re = std::vector<double>((int)sig.size(), 0);
	im = std::vector<double>((int)sig.size(), 0);
	power = std::vector<double>((int)sig.size(), 0);

	fft_excute(sig, re, im, 1); // フーリエ変換

	for (int i = 0; i < (int)power.size(); i++)power[i] = (re[i] * re[i] + im[i] * im[i]);
	
	re = power;
	im = std::vector<double>((int)re.size(), 0);
	fft_excute(sig, re, im, -1); // 逆フーリエ変換

	for (int i = 0; i < (int)r.size(); i++)r[i] = sig[i];

}

void LPC::calc_ACF_FFT(std::vector<double>& r_ptr, std::vector<double>& signal_ptr) {
	std::vector<double> sig = signal_ptr;

	for (int i = 0; i < Frame_L; i++)sig.push_back(0.0); // 0-padding

	re = std::vector<double>((int)sig.size(), 0);
	im = std::vector<double>((int)sig.size(), 0);
	
	fft_excute(sig, re, im, 1); // フーリエ変換
		
	for (int i = 0; i < (int)power.size(); i++)power[i] = (re[i] * re[i] + im[i] * im[i]);
	
	re = power;
	im = std::vector<double>((int)re.size(), 0);
	fft_excute(sig, re, im, -1); // 逆フーリエ変換

	for (int i = 0; i < (int)r_ptr.size(); i++)r_ptr[i] = sig[i];
}


void LPC::calc_Levinson_Durbin() {
	int k, j;

	a[0] = b[0] = 1.0;
	a[1] = -r[1] / r[0];
	b[1] = r[0] + r[1] * a[1];
	double lambda = -r[1] / r[0];

	std::vector<double>U(a.size() + 1);
	std::vector<double>V(a.size() + 1);


	for (k = 1; k < Order; k++) {
		lambda = 0.0;
		for (j = 0; j < k + 1; j++) {
			lambda -= a[j] * r[k + 1 - j];
		}
		lambda /= b[k];

		U[0] = 1.0;
		for (j = 1; j < k + 1; j++)U[j] = a[j];
		U[j] = 0.0;

		V[0] = 0.0;
		for (j = k; j > 0; j--)V[k - j + 1] = a[j];
		V[k - j + 1] = 1.0;
		
		for (int s = 0; s <= k+1; s++) {
			a[s] = U[s] + V[s] * lambda;
		}

		b[k + 1] = b[k] * (1.0 - lambda * lambda);
	}
}


void LPC::calc_error() {
	// 線形予測
	double tmp;
	// 補正あり,正規化
		
	for (int k = 0; k < Frame_L; k++) {
		tmp = 0;
		if (k < Order)y.push_back(signal[k]);
		
		else {
			for (int j = 1; j < (int)a.size(); j++) {
				tmp -= a[j] * (signal[k - j]);
			}
			y.push_back(tmp);
		}
	}

	//残差信号
	for (int k = 0; k < Frame_L; k++) {
			
		
		if (Order < k) {
			tmp = 0.0;
			for (int j = 0; j < (int)a.size(); j++) {
				tmp += a[j] * (signal[k - j]);
			}
			//e[k] = tmp;
			e[k] = tmp * (0.5 - 0.5*cos(2 * Pi*k / Frame_L)); // ハニング窓
		}
		
		e_rms += e[k]*e[k];
	}

	e_rms = sqrt(e_rms/(double)Frame_L);

}
