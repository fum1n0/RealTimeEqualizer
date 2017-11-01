#pragma once


#include"Filter.hpp"
#include"RecordPlay.hpp"
#include"Vocoder.hpp"

class Interface {

private:

	Font font;
	GUI gui;
	std::vector<double>X;
	std::shared_ptr<Filter>filter;
	std::shared_ptr<RecordPlay>recordplay;
	std::shared_ptr<Vocoder>vocoder;

public:

	Interface();
	void init();
	void update();
	void isReaction();
	void setText();
	int calcArea(int);
	void setCoefficient();
	void draw();


};