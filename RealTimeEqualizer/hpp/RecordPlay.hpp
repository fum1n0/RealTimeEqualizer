#pragma once

#include"Vocoder.hpp"

class RecordPlay {

private:
	
	Recorder recorder;

	Sound sound;
	
	const size_t BufferSize = 44100 * 10;
	size_t readPos;
	size_t writePos;
	size_t soundPos;
	size_t readSize;
	size_t writeSize;
	
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