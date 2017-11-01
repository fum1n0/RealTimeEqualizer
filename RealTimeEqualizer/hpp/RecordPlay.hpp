#pragma once

#include"Vocoder.hpp"

class RecordPlay {

private:
	const size_t BufferSize = 44100 * 10;
	Recorder recorder;
	Sound sound;
	size_t readPos;
	size_t writePos;

	size_t soundPos;
	size_t readSize;
	
	bool slow;

	std::vector<double>filter;

	Wave x;
	Wave y;
	double tmp;

	std::shared_ptr<Vocoder>vocoder;

public:	

	RecordPlay();
	void update();
	void setFilter(std::vector<double>);
	void setVocoder(std::shared_ptr<Vocoder>);
};