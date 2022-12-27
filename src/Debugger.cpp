#include "Debugger.h"
#include <iostream>
Debugger::Debugger()
{

}
Debugger::~Debugger()
{

}
void Debugger::Update()
{
	system("cls");
	std::cout << "|------------DEBUGGER------------|" << std::endl;
	std::cout << "| "<<"Current Algorithm: "<< currentAlg << std::endl;
	std::cout << "| " << "Min Nodes Explored: " << minExplored << std::endl;
	std::cout << "| " << "Max Nodes Explored: " << maxExplored << std::endl;
	std::cout << "|--------------------------------|" << std::endl;
	//std::cout << "|" << "Nodes Explored (Current Row): " << maxExplored << std::endl;
	//std::cout << "|--------------------------------|" << std::endl;
	
}
void Debugger::Reset()
{
	currentAlg = "NONE";
	minExplored = 0;
	maxExplored = 0;
	nowExplored = 0;

	Update();
}