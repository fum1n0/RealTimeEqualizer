#pragma once

#include"RecordPlay.hpp"

class Filter {

private:
	std::vector<double>freq;
	std::vector<double>signal;
	std::vector<double>filter;
	int degree;
	std::shared_ptr<RecordPlay>recordplay;

public:
	Filter(int, int);
	void update();
	void change(std::vector<double>, int);
	std::vector<double> ifft_execute();


};