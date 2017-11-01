#pragma once

#include"Generator.hpp"
#include"LPC.hpp"

class Vocoder : public LPC, Generator{

private:

	bool execute = false;




public:

	void setExecute(bool);


};
