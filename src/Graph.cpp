#include "Graph.h"
Graph::Graph(){}
Graph::~Graph() {}

void Graph::addConnection(Vector2D *fromNode, Vector2D *toNode, int xCell, int yCell, float cost)
{
	connections.emplace(std::pair<int,int>(xCell, yCell), new Connection(fromNode, toNode, cost));
}

void Graph::drawConnections() {

		std::pair <std::multimap<std::pair<int,int>, Connection*>::iterator, std::multimap<std::pair<int, int>, Connection*>::iterator> rangeIterator = connections.equal_range(std::pair<int, int>(0, 12));
		for (std::multimap<std::pair<int, int>, Connection*>::iterator it = rangeIterator.first; it != rangeIterator.second; ++it) {
			Vector2D* tmp = it->second->GetFromNode();
			std::cout << tmp->x << std::endl;
		}
		
		//SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), (int)connections[i]->GetFromNode()->x, (int)connections[i]->GetFromNode()->y, (int)connections[i]->GetToNode()->x, (int)connections[i]->GetToNode()->y);

}

std::vector<std::pair<Vector2D*,float>> Graph::getNextNodes(int i, int j) {

	std::vector<std::pair<Vector2D*, float>> nextNodes;
	std::pair <std::multimap<std::pair<int, int>, Connection*>::iterator, std::multimap<std::pair<int, int>, Connection*>::iterator> rangeIterator = connections.equal_range(std::pair<int, int>(i, j));
	for (std::multimap<std::pair<int, int>, Connection*>::iterator it = rangeIterator.first; it != rangeIterator.second; ++it) {
		nextNodes.push_back(std::pair <Vector2D*, float>(it->second->GetToNode(), it->second->GetCost()));
	}	
	return nextNodes;
}

std::vector<std::pair<Vector2D*, float>> Graph::getNextNodes(Vector2D v) {
	return getNextNodes((int)v.x / CELL_SIZE, (int)v.y / CELL_SIZE);	
}

Connection* Graph::getConnectionIn(Vector2D v) {
	std::multimap<std::pair<int, int>, Connection*>::iterator it = connections.find(std::pair<int, int> ((int)v.x / CELL_SIZE, (int)v.y / CELL_SIZE));
	return it->second;
}
