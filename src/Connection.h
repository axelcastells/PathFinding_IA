#pragma once
#include "Node.h"
class Connection
{
public:
	Connection();
	float GetCost();
	Node* GetFromNode();
	Node* GetToNode();
};