#pragma once

#include"../hpp/Interface.hpp"


void Main(){

	const Font font(20);
	ScalableWindow::Setup(600, 480);
	Window::SetTitle(L"Filter Editer");

	std::shared_ptr<Interface>ui = std::make_shared<Interface>();

	while (System::Update()) {

		ui->update();


	}
}
