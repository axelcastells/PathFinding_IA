#pragma once
#include "Vector2D.h"
class Connection
{
public:
	Connection(Vector2D* _fromNode, Vector2D* _toNode, float _cost);
	float GetCost();
	Vector2D* GetFromNode();
	Vector2D* GetToNode();
private:
	float cost;
	Vector2D* fromNode;
	Vector2D* toNode;
};