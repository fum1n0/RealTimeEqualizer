
#include"../hpp/FilterEditorFreq.hpp"


FilterEditorFreq::FilterEditorFreq() {

	gui = GUI(GUIStyle::Default);
	gui.setTitle(L"Config");
		
	gui.addln(L"filter", GUIButton::Create(L"フィルタ更新"));


	gui.add(L"hr", GUIHorizontalLine::Create(1));
	gui.horizontalLine(L"hr").style.color = Color(127);
		

	gui.addln(L"degree_num", GUIText::Create(L"", 300));
	gui.add(L"degree", GUISlider::Create(5, 13, 7, 100));

	gui.add(L"hr", GUIHorizontalLine::Create(1));
	gui.horizontalLine(L"hr").style.color = Color(127);
		
	
	X = std::vector<double>(pow(2,gui.slider(L"degree")._get_valueInt()), 1.0);
	
	font = Font(20);


}


void FilterEditorFreq::update() {

	isReaction();
	setText();
	draw();

}


void FilterEditorFreq::isReaction() {

	
	if (gui.button(L"filter").pushed); // Filter Update


	if (gui.slider(L"degree").hasChanged)X = std::vector<double>(pow(2,gui.slider(L"degree")._get_valueInt()), 1.0);

	
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


void FilterEditorFreq::setText() {
	
	gui.text(L"degree_num").text = Format(L"分解能: 2^", gui.slider(L"degree")._get_valueInt());
	
}


int FilterEditorFreq::calcArea(int x) {

	int index;
	double resolution;

	resolution = Window::Width() / (double)X.size();
	index = (int)(x / resolution);

	return index;
}

void FilterEditorFreq::setCoefficient(int index) {

	
	if (index < 0 || X.size() <= index)return;

	double amp = 1.0 - (Mouse::PosF().y / (double)Window::Height());

	if (1.0 < amp)amp = 1.0;
	else if (amp < 0.0)amp = 0.0;

	X[index] = amp;

}



void FilterEditorFreq::draw() {

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


std::vector<double> IFFT(std::vector<double> re) {






}

