#pragma once
#include <vector>

#define MAX_CONNECTED_NODES 8

class Node
{
public:
	Node();
	~Node();
	inline int GetCost() { return cost; };
private:
	int cost;
};