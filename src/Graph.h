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

private:
	//Pair column ordered, ordered (y,x)
	std::map<std::pair<int, int>, Connection*> connections;
	//std::vector<Connection*> connections;
};