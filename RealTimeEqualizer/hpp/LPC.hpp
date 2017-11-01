#pragma once

#include"Public.hpp"

class LPC{

protected:
		
	Wave wav_base; // 元波形
	Wave wav_pre; // 高音強調処理後波形
	Wave wav_zero; // 0-pound

	unsigned long long int length ; // 音声サンプル数
	int Frame_L = 1024; // フレーム長
	
	double pre_emphasis = 0.9; // 高音強調割合
	double de_emphasis = 0.9; // 低音強調割合

	double e_rms = 0.0; // 二乗平均平方根

	Wave wav_ana; // 解析用フレーム長音声切り出し部分
	
	const int Order = 46; // 解析次数
	
	std::vector<double>signal; // フレーム長切り出しシグナル

	std::vector<double>re; // 実数部分
	std::vector<double>im; // 虚数部分

	std::vector<double>amp; // Amp
	std::vector<double>power; //power
	std::vector<double>dbv; // dBV

	std::vector<double>r; // 自己相関関数

	std::vector<double>a; // 線形予測係数
	std::vector<double>b; // 誤差信号係数
	
	std::vector<double>y; // 予測信号

	std::vector<double>e; // 残差信号
	
public:
		

	void setLPC(Wave);
	void init();
	void calc_formant(Wave);
	
	void hanning_execute();
	void fft_excute(std::vector<double>&, std::vector<double>&, std::vector<double>&, int);
	void calc_ACF_FFT();
	void calc_ACF_FFT(std::vector<double>&, std::vector<double>&);
	void calc_Levinson_Durbin();
	
	void calc_error();
	

};