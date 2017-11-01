#pragma once

#include"../hpp/Interface.hpp"


Interface::Interface() {

	gui = GUI(GUIStyle::Default);
	gui.setTitle(L"Config");
		
	gui.addln(L"filter", GUIButton::Create(L"フィルタ更新"));
	
	gui.add(L"hr", GUIHorizontalLine::Create(1));
	gui.horizontalLine(L"hr").style.color = Color(127);
	
	gui.addln(L"degree_text", GUIText::Create(L"", 200));
	gui.add(L"degree_num", GUISlider::Create(5, 11, 8, 150));

	gui.add(L"hr", GUIHorizontalLine::Create(1));
	gui.horizontalLine(L"hr").style.color = Color(127);
		
	gui.addln(L"dimension_text", GUIText::Create(L"", 200));
	gui.add(L"dimension_num", GUISlider::Create(5, 50, 10, 150));

	gui.add(L"hr", GUIHorizontalLine::Create(1));
	gui.horizontalLine(L"hr").style.color = Color(127);

	gui.addln(L"vocoder_title", GUIText::Create(L"Vocoder", 200));
	gui.addln(L"isVocoder", GUIToggleSwitch::Create(L"オフ", L"オン", false));
	gui.addln(L"freq_text", GUIText::Create(L"", 150));
	gui.addln(L"freq_num", GUISlider::Create(50, 1000, 250, 150));
	gui.add(L"generator", GUIRadioButton::Create({L"ホワイトノイズ", L"ノコギリ波" }, 0u, true));
	

	X = std::vector<double>((int64)pow(2,gui.slider(L"degree_num")._get_valueInt()), 1.0);
	
	font = Font(20);

	filter = std::make_shared<Filter>((int)X.size(), (int)gui.slider(L"dimension_num")._get_valueInt());
	recordplay = std::make_shared<RecordPlay>();
	vocoder = std::make_shared<Vocoder>();
	this->init();

}


void Interface::init() {
	filter->update(X, (int)gui.slider(L"dimension_num")._get_valueInt());
	recordplay->setFilter(filter->getFilter());
	recordplay->setVocoder(vocoder);
	vocoder->setFreq(gui.slider(L"freq_num")._get_valueInt());
}


void Interface::update() {

	isReaction();
	recordplay->update();
	setText();
	draw();

}


void Interface::isReaction() {

	
	if (gui.button(L"filter").pushed) {
		filter->update(X, (int)gui.slider(L"dimension_num")._get_valueInt()); // Filter Update
		recordplay->setFilter(filter->getFilter());
	}

	if (gui.slider(L"degree_num").hasChanged)X = std::vector<double>((int64)pow(2,gui.slider(L"degree_num")._get_valueInt()), 1.0);
	
	if (gui.slider(L"degree_num").hasChanged || gui.slider(L"dimension_num").hasChanged) {
		if (pow(2, gui.slider(L"degree_num")._get_valueInt()) < 2 * gui.slider(L"dimension_num")._get_valueInt() + 1) {
			gui.slider(L"dimension_num").setValue((pow(2, gui.slider(L"degree_num")._get_valueInt())/2)-1);
		}
	}

	if (gui.slider(L"freq_num").hasChanged)vocoder->setFreq(gui.slider(L"freq_num")._get_valueInt());

	if (gui.toggleSwitch(L"isVocoder").isRight)vocoder->setExecute(true);
	else vocoder->setExecute(false);

	if (gui.radioButton(L"generator").hasChanged) {
		vocoder->setGenerateId((int)gui.radioButton(L"generator").checkedItem.value());
	}


	if (Input::MouseL.pressed){
		if (!gui.style.visible){
			setCoefficient();
		}
	}


	if (Input::MouseR.clicked) {
		if (gui.style.visible)gui.show(false);
		else gui.show(true);
	}
	

}


void Interface::setText() {
	
	gui.text(L"degree_text").text = Format(L"分割数 2^n: n = ", gui.slider(L"degree_num")._get_valueInt());
	gui.text(L"dimension_text").text = Format(L"次元数 2m+1: m = ", gui.slider(L"dimension_num")._get_valueInt());
	gui.text(L"freq_text").text = Format(L"周波数: ", gui.slider(L"freq_num")._get_valueInt(),L"Hz");
	// gui.radioButton(L"generator").num_items // ラジオボタンの総数
	//uint64 test = gui.radioButton(L"generator").checkedItem.value(); // 選択してるラジオボタンID
	
}


int Interface::calcArea(int x) {

	int index;
	double resolution;

	resolution = Window::Width() / (double)X.size();
	index = (int)(x / resolution);

	return index;
}


void Interface::setCoefficient() {


	std::pair<int, double>prev, now, low, high;
	double a; // 変化度

	prev.first = calcArea(Mouse::PreviousPos().x);
	now.first= calcArea(Mouse::Pos().x);
	
	if (prev.first < 0)prev.first = 0;
	else if ((int)X.size() - 1 < prev.first)prev.first = (int)X.size() - 1;

	if (now.first < 0)now.first = 0;
	else if ((int)X.size() - 1 < now.first)now.first = (int)X.size() - 1;

	prev.second = 1.0 - (Mouse::PreviousPosF().y / (double)Window::Height());
	if (1.0 < prev.second)prev.second = 1.0;
	else if (prev.second < 0.0)prev.second = 0.0;

	now.second = 1.0 - (Mouse::PosF().y / (double)Window::Height());
	if (1.0 < now.second)now.second = 1.0;
	else if (now.second < 0.0)now.second = 0.0;

	
	low = std::min(prev, now);
	high = std::max(prev, now);

	if (prev.first != now.first) {
		a = (high.second - low.second) / (double)(high.first - low.first);
		for (int i = low.first; i <= high.first; i++) {
			X[i] = low.second + a*(i-low.first);
		}

	}else{
		X[low.first] = low.second;
		X[high.first] = high.second;
	}


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

	/*font(vocoder->getFreq()).draw(text_x, Mouse::Pos().y - 100);
	font(recordplay->vocoder->getFreq()).draw(text_x, Mouse::Pos().y - 150);*/

}


