#include "Connection.h"

Connection::Connection(Vector2D* _fromNode, Vector2D* _toNode, float _cost) {
	fromNode = _fromNode;
	toNode = _toNode;
	cost = _cost;
}

float Connection::GetCost() {
	return cost;
}

Vector2D* Connection::GetFromNode() {
	return fromNode;
}

Vector2D* Connection::GetToNode() {
	return toNode;
}