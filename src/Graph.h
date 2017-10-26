#pragma once
#include "Connection.h"
#include "Vector2D.h"
#include <vector>
class Graph
{
public:
	Graph();
	~Graph();

	void AddNode(Vector2D node);

	void GenerateConnections();
	std::vector<Connection*> GetConnections(Vector2D fromNode);

private:
	std::vector<Vector2D> nodes;
	std::vector<Connection*> connections;
};