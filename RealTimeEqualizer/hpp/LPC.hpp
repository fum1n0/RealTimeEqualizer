#pragma once

#include"Public.hpp"

class LPC{

protected:
		
	Wave wav_base; // ���g�`
	Wave wav_pre; // ��������������g�`
	Wave wav_zero; // 0-pound

	unsigned long long int length ; // �����T���v����
	int Frame_L = 1024; // �t���[����
	
	double pre_emphasis = 0.9; // ������������
	double de_emphasis = 0.9; // �ቹ��������

	double e_rms = 0.0; // ��敽�ϕ�����

	Wave wav_ana; // ��͗p�t���[���������؂�o������
	
	const int Order = 46; // ��͎���
	
	std::vector<double>signal; // �t���[�����؂�o���V�O�i��

	std::vector<double>re; // ��������
	std::vector<double>im; // ��������

	std::vector<double>amp; // Amp
	std::vector<double>power; //power
	std::vector<double>dbv; // dBV

	std::vector<double>r; // ���ȑ��֊֐�

	std::vector<double>a; // ���`�\���W��
	std::vector<double>b; // �덷�M���W��
	
	std::vector<double>y; // �\���M��

	std::vector<double>e; // �c���M��
	
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