#pragma once

#include"../hpp/Public.hpp"
#include"../hpp/Filter.hpp"

class Interface {

private:

	Font font;
	GUI gui;
	std::vector<double>X;
	std::vector<double>signal;
	std::shared_ptr<Filter>filter;

public:

	Interface();

	void update();
	void isReaction();
	void setText();
	int calcArea(int);
	void setCoefficient(int);
	void draw();


};