#pragma once

#pragma once

#include<Siv3D.hpp>
#include<iostream>
#include<vector>
#include<unordered_map>
#include<memory>
#include<numeric>
#include<HamFramework.hpp>
#include <iomanip>

class FilterEditorFreq {

private:

	Font font;
	GUI gui;
	std::vector<double>X;
	std::vector<double>signal;
	std::vector<double>filter;

public:

	FilterEditorFreq();

	void update();
	void isReaction();
	void setText();
	int calcArea(int);
	void setCoefficient(int);
	void draw();


};