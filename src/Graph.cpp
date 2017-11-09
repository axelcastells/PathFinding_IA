#include "Graph.h"
Graph::Graph(){}
Graph::~Graph() {}

void Graph::addConnection(Vector2D *fromNode, Vector2D *toNode, int yCell, int xCell, float cost)
{
	std::pair<int, int> temp;
	temp.first = yCell;
	temp.second = xCell;
	connections.emplace(temp, new Connection(fromNode, toNode, cost));
	//connections.push_back(new Connection(fromNode, toNode, cost));
}

void Graph::drawConnections() {
	for (int i = 0; i < connections.size(); i++) {
		
		//SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), (int)connections[i]->GetFromNode()->x, (int)connections[i]->GetFromNode()->y, (int)connections[i]->GetToNode()->x, (int)connections[i]->GetToNode()->y);
	}
}
