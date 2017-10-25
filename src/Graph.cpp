#include "Graph.h"
Graph::Graph(){}
Graph::Graph(std::vector<std::vector<int>> terrain)
{
	for(int i = 0; i < (int)terrain.size(); i++)
	{
		for (int j = 0; j < (int)terrain[i].size(); j++)
		{
			if (terrain[i][j] == 1)
			{
				nodes[i][j] = new Node;
			}
		}
	}
}

Graph::~Graph() {}

void Graph::setGraph(std::vector<std::vector<int>> terrain){
	for (int i = 0; i < (int)terrain.size(); i++)
	{
		for (int j = 0; j < (int)terrain[i].size(); j++)
		{
			if (terrain[i][j] == 1)
			{
				Node* dummy;
				nodes[i].push_back(dummy);
			}
		}
	}
}

std::vector<Connection*> Graph::GetConnections(Node* n)
{
	return std::vector<Connection*>();
}