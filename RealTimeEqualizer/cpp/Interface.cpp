#pragma once

#include"../hpp/Interface.hpp"


Interface::Interface() {

	gui = GUI(GUIStyle::Default);
	gui.setTitle(L"Config");
		
	gui.addln(L"filter", GUIButton::Create(L"フィルタ更新"));


	gui.add(L"hr", GUIHorizontalLine::Create(1));
	gui.horizontalLine(L"hr").style.color = Color(127);
		

	gui.addln(L"degree_num", GUIText::Create(L"", 300));
	gui.add(L"degree", GUISlider::Create(5, 11, 8, 100));

	gui.add(L"hr", GUIHorizontalLine::Create(1));
	gui.horizontalLine(L"hr").style.color = Color(127);
		
	gui.addln(L"dimension_num", GUIText::Create(L"", 300));
	gui.add(L"dimension", GUISlider::Create(5, 50, 10, 100));

	gui.add(L"hr", GUIHorizontalLine::Create(1));
	gui.horizontalLine(L"hr").style.color = Color(127);


	X = std::vector<double>((int64)pow(2,gui.slider(L"degree")._get_valueInt()), 1.0);
	
	font = Font(20);

	filter = std::make_shared<Filter>((int)X.size(), (int)gui.slider(L"dimension")._get_valueInt());
	filter->change(X, (int)gui.slider(L"dimension")._get_valueInt());
}


void Interface::update() {

	isReaction();
	filter->update();
	setText();
	draw();

}


void Interface::isReaction() {

	
	if (gui.button(L"filter").pushed)filter->change(X, (int)gui.slider(L"dimension")._get_valueInt()); // Filter Update


	if (gui.slider(L"degree").hasChanged)X = std::vector<double>((int64)pow(2,gui.slider(L"degree")._get_valueInt()), 1.0);

	

	
	if (Input::MouseL.pressed) {
		if (!gui.style.visible) {
			//Mouse::PreviousPos();
			setCoefficient(calcArea(Mouse::Pos().x));
		}
	}


	if (Input::MouseR.clicked) {
		if (gui.style.visible)gui.show(false);
		else gui.show(true);
	}

	

}


void Interface::setText() {
	
	gui.text(L"degree_num").text = Format(L"階調数 2^n: n = ", gui.slider(L"degree")._get_valueInt());
	gui.text(L"dimension_num").text = Format(L"次元数 2m+1: m = ", gui.slider(L"dimension")._get_valueInt());

}


int Interface::calcArea(int x) {

	int index;
	double resolution;

	resolution = Window::Width() / (double)X.size();
	index = (int)(x / resolution);

	return index;
}

void Interface::setCoefficient(int index) {

	
	if (index < 0 || X.size() <= index)return;

	double amp = 1.0 - (Mouse::PosF().y / (double)Window::Height());

	if (1.0 < amp)amp = 1.0;
	else if (amp < 0.0)amp = 0.0;

	X[index] = amp;

}



void Interface::draw() {

	double resolution_x = Window::Width() / (double)X.size();
	std::string str;
	int index, text_x;

	// block
	for (unsigned int i = 0; i < X.size(); i++) {
		RectF(i * resolution_x, Window::Height() , resolution_x, -Window::Height()*X[i]).draw(HSV(0.0, 1.0, 1.0));
	}

	
	// now_position
	index = calcArea(Mouse::Pos().x);
	if (index < 2 * X.size() / 3)text_x = Mouse::Pos().x + 15;
	else text_x = Mouse::Pos().x - 125;

	if (0 <= index && index < X.size()) {
		str = std::to_string(X[index]);
		str = str.substr(0, 5);
		font(L"X[" + Widen(std::to_string(index)) + L"]").draw(text_x, Mouse::Pos().y - 15);
		font(Widen(str)).draw(text_x, Mouse::Pos().y + 15);
	}
}


