#pragma once

#include"RecordPlay.hpp"

class Filter {

private:
	std::vector<double>freq;
	std::vector<double>signal;
	std::vector<double>filter;
	int degree;
	

public:
	Filter(int, int);
	void update(std::vector<double>, int);
	std::vector<double> ifft_execute();
	std::vector<double> getFilter();

};