#pragma once
#include "Graph.h"
#include "Connection.h"
#include "Path.h"
#include <queue>
#include "utils.h"

class PathFinder
{
public:
	PathFinder(Path* p, Graph* g);
	PathFinder(Graph* g);
	PathFinder();
	~PathFinder() {}
	bool pathFound;

	void BFS(Vector2D *startPoint, Vector2D *targetPoint); //Breadth First Search
	void Dijkstra(Vector2D *startPoint, Vector2D *targetPoint); //Dijkstra Search
	void Greedy(Vector2D *startPoint, Vector2D *targetPoint); //Greedy Best First Search
	void AStar(Vector2D *startPoint, Vector2D *targetPoint); //AStar search

	float heuristic(Vector2D a, Vector2D b);

private:
	Graph* graph;
	Path* path;
};