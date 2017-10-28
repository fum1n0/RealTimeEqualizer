
#include"../hpp/FilterEditorFreq.hpp"


void Main(){

	const Font font(20);
	ScalableWindow::Setup(600, 480);
	Window::SetTitle(L"Filter Editer");

	std::shared_ptr<FilterEditorFreq>fef = std::make_shared<FilterEditorFreq>();

	while (System::Update()) {

		fef->update();


	}
}
