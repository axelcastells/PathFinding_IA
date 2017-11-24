#pragma once
#include <string>

class Debugger
{

public:
	Debugger();
	~Debugger();

	std::string currentAlg;
	float maxExplored;
	float minExplored;
	float nowExplored;

	void Reset();
	void Update();

};