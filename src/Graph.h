#pragma once
#include "Connection.h"
#include "Vector2D.h"
#include <SDL.h>
#include <SDL_image.h>
#include "SDL_SimpleApp.h"
#include <map>
#include <vector>

class Graph
{
public:
	Graph();
	~Graph();
	void addConnection(Vector2D *fromNode, Vector2D *toNode, int yCell, int xCell, float cost);
	void drawConnections();
	std::vector<std::pair<Vector2D*, float>> getNextNodes(int i, int j);
	std::vector<std::pair<Vector2D*, float>> getNextNodes(Vector2D v);
	Connection* getConnectionIn(Vector2D v);

	

private:
	//Pair column ordered, ordered (y,x)
	std::multimap<std::pair<int, int>, Connection*> connections;
};