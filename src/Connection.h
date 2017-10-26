#pragma once
#include "Vector2D.h"
class Connection
{
public:
	Connection();
	float GetCost();
	Vector2D* GetFromNode();
	Vector2D* GetToNode();
private:
	float cost;
	Vector2D* fromNode;
	Vector2D* toNode;
};