#include "Graph.h"
Graph::Graph(){}
Graph::~Graph() {}

void Graph::AddNode(Vector2D node)
{
	nodes.push_back(node);
}
std::vector<Connection*> Graph::GetConnections(Vector2D n)
{
	return std::vector<Connection*>();
}
void Graph::GenerateConnections()
{

}