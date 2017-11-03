#include "PathFinder.h"

PathFinder::PathFinder(Path* path, Graph* g)
{
	graph = g;
}

void PathFinder::BFS(Vector2D startPoint, Vector2D targetPoint)
{
	std::queue<Connection*> frontier;
	
}