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
	frontierRects.clear();
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
				SDL_Rect rect = { next.first->x - CELL_SIZE / 2, next.first->y - CELL_SIZE / 2, CELL_SIZE, CELL_SIZE};
				frontierRects.push_back(rect);
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
		//draw_circle(TheApp::Instance()->getRenderer(), (int)current->x, (int)current->y, 14, 255, 255, 255, 255);
	}

	for (int i = pathPoints.size() - 1; i >= 0; i--) {
		path->points.push_back(pathPoints[i]);
	}
	
	pathFound = true;
	
}

void PathFinder::Dijkstra(Vector2D *startPoint, Vector2D *targetPoint)
{
	frontierRects.clear();
	pathFound = false;

	std::priority_queue<std::pair<Vector2D*,float>> frontier;
	frontier.push(std::pair<Vector2D*,float>(startPoint, 0));

	std::map<std::pair<float, float>, Vector2D*> visited;
	std::map<std::pair<float,float>, float> costSoFar;

	visited.emplace(std::pair<float, float>(startPoint->x, startPoint->y), startPoint);
	costSoFar.emplace(std::pair<float, float>(startPoint->x, startPoint->y), 0);

	Vector2D *current;

	while (!frontier.empty()) {
		current = frontier.top().first;
		frontier.pop();

		if (*current == *targetPoint) {
			break;
		}

		for each (std::pair<Vector2D*, float> next in graph->getNextNodes(*current)) {
			float newCost = costSoFar[std::pair<float, float>(current->x, current->y)] + next.second;
			if (costSoFar.count(std::pair<float, float>(next.first->x, next.first->y)) == 0 || newCost < costSoFar[std::pair<float,float>(next.first->x,next.first->y)]) {
				costSoFar.emplace(std::pair<float, float>(next.first->x, next.first->y), newCost);
				frontier.push(std::pair<Vector2D*, float>(next.first, -newCost));
				SDL_Rect rect = { next.first->x - CELL_SIZE / 2, next.first->y - CELL_SIZE / 2, CELL_SIZE, CELL_SIZE };
				frontierRects.push_back(rect);
				visited.emplace(std::pair<float, float>(next.first->x, next.first->y), current);
				draw_circle(TheApp::Instance()->getRenderer(), (int)next.first->x, (int)next.first->y, 14, 255, 0, 0, 255);

			}
		}

	}

	current = targetPoint;
	std::vector<Vector2D> pathPoints;
	pathPoints.push_back(*current);

	while (*current != *startPoint) {
		current = visited[std::pair<float, float>(current->x, current->y)];
		pathPoints.push_back(*current);
		//draw_circle(TheApp::Instance()->getRenderer(), (int)current->x, (int)current->y, 14, 255, 255, 255, 255);
	}

	for (int i = pathPoints.size() - 1; i >= 0; i--) {
		path->points.push_back(pathPoints[i]);
	}

	pathFound = true;
}

void PathFinder::Greedy(Vector2D *startPoint, Vector2D *targetPoint) {
	frontierRects.clear();
	pathFound = false;

	std::priority_queue<std::pair<Vector2D*, float>> frontier;
	frontier.push(std::pair<Vector2D*, float>(startPoint, 0));

	std::map<std::pair<float, float>, Vector2D*> visited;

	visited.emplace(std::pair<float, float>(startPoint->x, startPoint->y), startPoint);

	Vector2D *current;

	while (!frontier.empty()) {
		current = frontier.top().first;
		frontier.pop();

		if (*current == *targetPoint) {
			break;
		}

		for each (std::pair<Vector2D*, float> next in graph->getNextNodes(*current)) {

			if (visited.count(std::pair<float, float>(next.first->x, next.first->y)) == 0 ) {
				float priority = heuristic(*targetPoint, *next.first);
				frontier.push(std::pair<Vector2D*, float>(next.first, -priority));
				SDL_Rect rect = { next.first->x - CELL_SIZE / 2, next.first->y - CELL_SIZE / 2, CELL_SIZE, CELL_SIZE };
				frontierRects.push_back(rect);
				visited.emplace(std::pair<float, float>(next.first->x, next.first->y), current);
				draw_circle(TheApp::Instance()->getRenderer(), (int)next.first->x, (int)next.first->y, 14, 255, 0, 0, 255);

			}
		}

	}

	current = targetPoint;
	std::vector<Vector2D> pathPoints;
	pathPoints.push_back(*current);

	while (*current != *startPoint) {
		current = visited[std::pair<float, float>(current->x, current->y)];
		pathPoints.push_back(*current);
		//draw_circle(TheApp::Instance()->getRenderer(), (int)current->x, (int)current->y, 14, 255, 255, 255, 255);
	}

	for (int i = pathPoints.size() - 1; i >= 0; i--) {
		path->points.push_back(pathPoints[i]);
	}

	pathFound = true;

}


void PathFinder::AStar(Vector2D *startPoint, Vector2D *targetPoint)
{
	frontierRects.clear();
	pathFound = false;

	std::priority_queue<std::pair<Vector2D*, float>> frontier;
	frontier.push(std::pair<Vector2D*, float>(startPoint, 0));

	std::map<std::pair<float, float>, Vector2D*> visited;
	std::map<std::pair<float, float>, float> costSoFar;

	visited.emplace(std::pair<float, float>(startPoint->x, startPoint->y), startPoint);
	costSoFar.emplace(std::pair<float, float>(startPoint->x, startPoint->y), 0);

	Vector2D *current;

	while (!frontier.empty()) {
		current = frontier.top().first;
		frontier.pop();

		if (*current == *targetPoint) {
			break;
		}

		for each (std::pair<Vector2D*, float> next in graph->getNextNodes(*current)) {
			float newCost = costSoFar[std::pair<float, float>(current->x, current->y)] + next.second;
			if (costSoFar.count(std::pair<float, float>(next.first->x, next.first->y)) == 0 || newCost < costSoFar[std::pair<float, float>(next.first->x, next.first->y)]) {
				costSoFar.emplace(std::pair<float, float>(next.first->x, next.first->y), newCost);
				float priority = heuristic(*targetPoint, *next.first) + newCost;
				frontier.push(std::pair<Vector2D*, float>(next.first, -priority));
				SDL_Rect rect = { next.first->x - CELL_SIZE / 2, next.first->y - CELL_SIZE / 2, CELL_SIZE, CELL_SIZE };
				frontierRects.push_back(rect);
				visited.emplace(std::pair<float, float>(next.first->x, next.first->y), current);
				draw_circle(TheApp::Instance()->getRenderer(), (int)next.first->x, (int)next.first->y, 14, 255, 0, 0, 255);

			}
		}

	}

	current = targetPoint;
	std::vector<Vector2D> pathPoints;
	pathPoints.push_back(*current);

	while (*current != *startPoint) {
		current = visited[std::pair<float, float>(current->x, current->y)];
		pathPoints.push_back(*current);
		//draw_circle(TheApp::Instance()->getRenderer(), (int)current->x, (int)current->y, 14, 255, 255, 255, 255);
	}

	for (int i = pathPoints.size() - 1; i >= 0; i--) {
		path->points.push_back(pathPoints[i]);
	}

	pathFound = true;
}

void PathFinder::MultiTargetAStar(Vector2D *startPoint, std::vector<Vector2D*> waypoints)
{
	frontierRects.clear();
	std::map<float, Vector2D*> tempReorder;
	Vector2D *temp = startPoint;
	std::vector<Vector2D*>::iterator it;
	

	while(!waypoints.empty())
	{
		std::pair<int, float> lastIndexedHeuristics = std::pair<int, float>(0, 100000);// index | heuristic

		for (int j = 0; j < waypoints.size(); j++)
		{
			float tempHeur = heuristic(*temp, *waypoints[j]);
			if (lastIndexedHeuristics.second > tempHeur)
			{
				lastIndexedHeuristics.first = j;
				lastIndexedHeuristics.second = tempHeur;
			}
		}

		tempReorder.insert(std::pair<float, Vector2D*>(lastIndexedHeuristics.second, waypoints[lastIndexedHeuristics.first]));
		temp = waypoints[lastIndexedHeuristics.first];

		it = waypoints.begin();
		std::advance(it, lastIndexedHeuristics.first);
		waypoints.erase(it);
	}

	for (std::map<float, Vector2D*>::iterator it = tempReorder.begin(); it != tempReorder.end(); it++)
	{
		waypoints.push_back(it->second);
	}

	temp = startPoint;
	for (int i = 0; i < waypoints.size(); i++) {
		AStar(temp, waypoints[i]);
		temp = waypoints[i];
	}
}

float PathFinder::heuristic(Vector2D _a, Vector2D _b) {
	Vector2D a((float)((int)_a.x / CELL_SIZE), (float)((int)_a.y / CELL_SIZE));
	Vector2D b((float)((int)_b.x / CELL_SIZE), (float)((int)_b.y / CELL_SIZE));
	return (float)(abs(a.x - b.x) + abs(a.y - b.y));
}

bool operator<(const std::pair<Vector2D*, float>& c1, const std::pair<Vector2D*, float>& c2) {
	return c1.second < c2.second;
}

std::vector<SDL_Rect> PathFinder::returnFrontier() {
	return frontierRects;
}