#pragma once
#include <iostream>
#include <minmax.h>
#include <SDL.h>
#include <SDL_image.h>
#include "SDL_SimpleApp.h"
#include "Vector2D.h"
#include "utils.h"
#include "SteeringBehavior.h"
#include "PathFinder.h"

#define DEFAULT_ALG Agent::SearchAlgorithm::BFS

class Agent
{
	friend class SteeringBehavior;

public:
	enum SearchAlgorithm
	{
		BFS, DIJKSTRA, GREEDY, ASTAR, WAYPOINTS_ASTAR
	};

private:
	SteeringBehavior *steering_behavior;
	Vector2D position;
	Vector2D velocity;
	Vector2D target;
	std::vector<Vector2D*> multiTargetList;



	float mass;
	float orientation;
	float max_force;
	float max_velocity;

	SDL_Color color;

	SDL_Texture *sprite_texture;
	bool draw_sprite;
	int sprite_num_frames;
	int sprite_w;
	int sprite_h;

public:
	Agent(SearchAlgorithm alg);
	~Agent();
	SteeringBehavior *Behavior();
	Vector2D getPosition();
	Vector2D getTarget();
	Vector2D getVelocity();
	float getMaxVelocity();
	void setPosition(Vector2D position);
	void setTarget(Vector2D target);
	void setMultiTarget(std::vector<Vector2D*> multiTargetList);
	void setVelocity(Vector2D velocity);
	void setMass(float mass);
	void setColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	void update(Vector2D steering_force, float dtime, SDL_Event *event);
	void draw();
	bool Agent::loadSpriteTexture(char* filename, int num_frames=1);
	void SetPathFinderGraph(Path* p, Graph* g);
	bool searchActive;
	PathFinder *pathFinder;
	SearchAlgorithm currentAlgorithm = DEFAULT_ALG;
};
