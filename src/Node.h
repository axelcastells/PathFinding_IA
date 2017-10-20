#pragma once
#include <vector>

#define MAX_CONNECTED_NODES 8

class Node
{
public:
	Node();
	~Node();
	inline std::vector<Node*> GetNodes() { return connectedNodes; };
	inline int GetCost() { return cost; };
private:
	std::vector<Node*> connectedNodes;
	int cost;
};