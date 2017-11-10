#include "PathFinder.h"

PathFinder::PathFinder(Path* path, Graph* g)
{
	graph = g;
}

PathFinder::PathFinder(Graph* g)
{
	graph = g;
}

PathFinder::PathFinder(){ }

void PathFinder::BFS(Vector2D *startPoint, Vector2D *targetPoint)
{
	std::queue<Vector2D*> frontier;
	frontier.push(startPoint);

	std::map<Vector2D*, bool> visited;

	visited.emplace(startPoint, true);

	Vector2D *current;

	while (!frontier.empty()) {
		current = frontier.front();
		frontier.pop();

		if (*current == *targetPoint) {
			break;
		}

 		for each (std::pair<Vector2D*, float> next in graph->getNextNodes(*current)) {			

			if (visited.count(next.first) == 0) {
				frontier.push(next.first);
				visited.emplace(next.first, true);
				draw_circle(TheApp::Instance()->getRenderer(), (int)next.first->x, (int)next.first->y, 14, 255, 0, 0, 255);
			}
		}
	}
	
}