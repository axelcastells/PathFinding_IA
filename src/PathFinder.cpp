#include "PathFinder.h"

PathFinder::PathFinder(Path* p, Graph* g)
{
	path = p;
	graph = g;
}

PathFinder::PathFinder(Graph* g)
{
	graph = g;
}

PathFinder::PathFinder(){ }

void PathFinder::BFS(Vector2D *startPoint, Vector2D *targetPoint)
{
	pathFound = false;

	std::queue<Vector2D*> frontier;
	frontier.push(startPoint);

	std::map<std::pair<float,float>, Vector2D*> visited;

	visited.emplace(std::pair<float, float> (startPoint->x, startPoint->y), startPoint);

	Vector2D *current;

	while (!frontier.empty()) {
		current = frontier.front();
		frontier.pop();

		if (*current == *targetPoint) {
			break;
		}

 		for each (std::pair<Vector2D*, float> next in graph->getNextNodes(*current)) {			

			if (visited.count(std::pair<float, float>(next.first->x, next.first->y)) == 0) {
				frontier.push(next.first);
				visited.emplace(std::pair<float, float>(next.first->x, next.first->y), current);
				draw_circle(TheApp::Instance()->getRenderer(), (int)next.first->x, (int)next.first->y, 14, 255, 0, 0, 255);

			}
		}

	}

	current = targetPoint;
	std::vector<Vector2D> pathPoints;
	pathPoints.push_back(*current);

	while (*current != *startPoint) {
		current = visited[std::pair<float,float>(current->x, current->y)];
		pathPoints.push_back(*current);
		draw_circle(TheApp::Instance()->getRenderer(), (int)current->x, (int)current->y, 14, 255, 255, 255, 255);
	}

	for (int i = pathPoints.size() - 1; i >= 0; i--) {
		path->points.push_back(pathPoints[i]);
	}
	
	pathFound = true;
	
}