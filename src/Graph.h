#pragma once
#include "Connection.h"
#include "Vector2D.h"
#include <SDL.h>
#include <SDL_image.h>
#include "SDL_SimpleApp.h"
#include <vector>
class Graph
{
public:
	Graph();
	~Graph();
	void addConnection(Vector2D *fromNode, Vector2D *toNode, float cost);
	void drawConnections();

private:
	std::vector<Connection*> connections;
};