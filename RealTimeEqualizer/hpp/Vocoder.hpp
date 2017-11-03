#pragma once

#include"Generator.hpp"
#include"LPC.hpp"

class Vocoder : public LPC, public Generator{

private:

	bool doExecute = false;
	int generateId;

public:

	void setExecute(bool);
	void setGenerateId(int);
	bool isExecute();

	Wave execute(Wave);

};
