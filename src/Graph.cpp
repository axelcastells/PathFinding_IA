#include "Graph.h"
Graph::Graph(){}
Graph::Graph(std::vector<std::vector<int>> terrain)
{
	for(int i = 0; i < terrain.size; i++)
	{
		for (int j = 0; j < terrain[i].size; j++)
		{
			if (terrain[i][j] == 1)
			{
				nodes[i][j] = new Node;
			}
		}
	}

}
std::vector<Node*> Graph::GetConnections(Node* n)
{
	
}