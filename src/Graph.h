#pragma once
#include "Node.h"
#include "Connection.h"
class Graph
{
public:
	Graph();
	Graph(std::vector<std::vector<int>> terrain);

	std::vector<Connection*> GetConnections(Node* fromNode);
	void setGraph(std::vector<std::vector<int>> terrain);
	~Graph();
private:
	std::vector<std::vector<Node*>> nodes;
};