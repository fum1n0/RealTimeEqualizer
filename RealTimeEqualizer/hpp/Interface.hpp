#pragma once


#include"Filter.hpp"

class Interface {

private:

	Font font;
	GUI gui;
	std::vector<double>X;
	std::shared_ptr<Filter>filter;

public:

	Interface();

	void update();
	void isReaction();
	void setText();
	int calcArea(int);
	void setCoefficient();
	void draw();


};