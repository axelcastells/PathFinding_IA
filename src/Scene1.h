#pragma once
#include <vector>
#include <time.h>
#include "Scene.h"
#include "Agent.h"
#include "Path.h"
#include "Graph.h"

class Scene1 :
	public Scene
{
public:
	Scene1(Agent::SearchAlgorithm alg);
	~Scene1();
	void update(float dtime, SDL_Event *event);
	void draw();
	const char* getTitle();

	// Rects to draw frontier
	std::vector<SDL_Rect> frontier_rects;

private:
	std::vector<Agent*> agents;
	Vector2D coinPosition;
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
	void drawTerrains();

	SDL_Texture *background_texture;
	SDL_Texture *coin_texture;
	void initMaze();

	// Diferents pesos
	void initTerrains();
	std::vector<std::pair<SDL_Rect, float>> terrains;

	bool loadTextures(char* filename_bg, char* filename_coin);
	std::vector< std::vector<int> > terrain;
	Vector2D cell2pix(Vector2D cell);
	Vector2D pix2cell(Vector2D pix);
	bool isValidCell(Vector2D cell);
	Graph terrainGraph;
};