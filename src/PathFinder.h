#pragma once
#include "Graph.h"
#include "Connection.h"
#include "Path.h"
#include <queue>

class PathFinder
{
public:
	PathFinder(Path* path, Graph* g);
	~PathFinder();

	void BFS(Vector2D startPoint, Vector2D targetPoint);//Breadth First Search
private:
	Graph* graph;
};