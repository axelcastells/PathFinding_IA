#pragma once
#include <vector>
#include <time.h>
#include "Scene.h"
#include "Agent.h"
#include "Path.h"
#include "Graph.h"

#define COINS 3

class Scene_WaypointPathFinding :
	public Scene
{
public:
	Scene_WaypointPathFinding(Agent::SearchAlgorithm alg);
	~Scene_WaypointPathFinding();
	void update(float dtime, SDL_Event *event);
	void draw();
	const char* getTitle();

	// Rects to draw frontier
	std::vector<SDL_Rect> frontier_rects;

private:
	std::vector<Agent*> agents;
	std::vector<Vector2D*> coinsPositions;
	float currentTargetCoinIndex;
	Vector2D currentTarget;
	int currentTargetIndex;
	Path path;
	int num_cell_x;
	int num_cell_y;
	bool draw_grid;
	std::vector<SDL_Rect> maze_rects;
	void drawMaze();
	void drawCoin();
	void drawFrontier();
	SDL_Texture *background_texture;
	SDL_Texture *coin_texture;
	void initMaze();
	// Diferents pesos
	void initTerrains();
	std::vector<SDL_Rect> terrains;

	bool loadTextures(char* filename_bg, char* filename_coin);
	std::vector< std::vector<int> > terrain;
	Vector2D cell2pix(Vector2D cell);
	Vector2D pix2cell(Vector2D pix);
	bool isValidCell(Vector2D cell);
	Graph terrainGraph;
};