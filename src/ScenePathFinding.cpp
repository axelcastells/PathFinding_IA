#include "ScenePathFinding.h"

using namespace std;

Scene_WaypointPathFinding::Scene_WaypointPathFinding(Agent::SearchAlgorithm alg)
{
	draw_grid = false;

	num_cell_x = SRC_WIDTH / CELL_SIZE;
	num_cell_y = SRC_HEIGHT / CELL_SIZE;
	terrainGraph = Graph();
	initTerrains();
	initMaze();

	loadTextures("../res/maze.png", "../res/coin.png");
	srand((unsigned int)time(NULL));
	Agent *agent = new Agent(alg);
	agent->loadSpriteTexture("../res/soldier.png", 4);
	agents.push_back(agent);


	// set agent position coords to the center of a random cell
	Vector2D rand_cell(-1, -1);
	while (!isValidCell(rand_cell))
		rand_cell = Vector2D((float)(rand() % num_cell_x), (float)(rand() % num_cell_y));
	agents[0]->setPosition(cell2pix(rand_cell));
	agents[0]->SetPathFinderGraph(&path, &terrainGraph);

	// set the coin in a random cell (but at least 3 cells far from the agent)
	for (int i = 0; i < COINS; i++)
	{
		coinsPositions.push_back(new Vector2D(-1, -1));
		while ((!isValidCell(*coinsPositions[i])) || (Vector2D::Distance(*coinsPositions[i], rand_cell)<3))
			coinsPositions[i] = new Vector2D((float)(rand() % num_cell_x), (float)(rand() % num_cell_y));
	}

	std::vector<Vector2D*> tempCoins;
	for (int i = 0; i < coinsPositions.size(); i++)
	{
		tempCoins.push_back(new Vector2D(cell2pix(*coinsPositions[i])));
	}

	agents[0]->setMultiTarget(tempCoins);

	// PathFollowing next Target
	currentTarget = Vector2D(0, 0);
	currentTargetIndex = -1;
	agents[0]->searchActive = true;

}

Scene_WaypointPathFinding::~Scene_WaypointPathFinding()
{
	if (background_texture)
		SDL_DestroyTexture(background_texture);
	if (coin_texture)
		SDL_DestroyTexture(coin_texture);

	for (int i = 0; i < (int)agents.size(); i++)
	{
		delete agents[i];
	}
}

void Scene_WaypointPathFinding::update(float dtime, SDL_Event *event)
{
	
	// Coin found detection
	for (int i = 0; i < coinsPositions.size(); i++) {
		if (pix2cell(agents[0]->getPosition()) == *coinsPositions[i] || abs(Vector2D::Distance(pix2cell(currentTarget), *coinsPositions[i])) < 1.0) {
			coinsPositions.erase(coinsPositions.begin() + i);
		}
	}

	if (coinsPositions.size() == 0) {
		// set agent position coords to the center of a random cell
		Vector2D rand_cell(-1, -1);
		while (!isValidCell(rand_cell))
			rand_cell = Vector2D((float)(rand() % num_cell_x), (float)(rand() % num_cell_y));

		path.points.clear();
		currentTargetIndex = -1;
		agents[0]->setVelocity(Vector2D(0, 0));

		agents[0]->SetPathFinderGraph(&path, &terrainGraph);

		// set the coin in a random cell (but at least 3 cells far from the agent)
		for (int i = 0; i < COINS; i++)
		{
			coinsPositions.push_back(new Vector2D(-1, -1));
			while ((!isValidCell(*coinsPositions[i])) || (Vector2D::Distance(*coinsPositions[i], rand_cell)<3))
				coinsPositions[i] = new Vector2D((float)(rand() % num_cell_x), (float)(rand() % num_cell_y));
		}

		std::vector<Vector2D*> tempCoins;
		for (int i = 0; i < coinsPositions.size(); i++)
		{
			tempCoins.push_back(new Vector2D(cell2pix(*coinsPositions[i])));
		}

		agents[0]->setMultiTarget(tempCoins);
	}

	/* Keyboard & Mouse events */

	switch (event->type) {
	case SDL_KEYDOWN:
		if (event->key.keysym.scancode == SDL_SCANCODE_SPACE)
			draw_grid = !draw_grid;

		break;
	default:
		break;
	}
	if ((currentTargetIndex == -1) && (path.points.size() > 0)) {
		currentTargetIndex = 0;
	}


	if (currentTargetIndex >= 0)
	{
		float dist = Vector2D::Distance(agents[0]->getPosition(), path.points[currentTargetIndex]);
		if (dist < path.ARRIVAL_DISTANCE)
		{
			if (currentTargetIndex == path.points.size() - 1)
			{
				if (dist < 3)
				{
					path.points.clear();
					currentTargetIndex = -1;
					agents[0]->setVelocity(Vector2D(0, 0));
					

				}
				else
				{
					Vector2D steering_force = agents[0]->Behavior()->Arrive(agents[0], currentTarget, path.ARRIVAL_DISTANCE, dtime);
					agents[0]->update(steering_force, dtime, event);

				}
				return;
			}
			currentTargetIndex++;
		}

		currentTarget = path.points[currentTargetIndex];
		Vector2D steering_force = agents[0]->Behavior()->Seek(agents[0], currentTarget, dtime);
		agents[0]->update(steering_force, dtime, event);

		if (currentTargetIndex >= 0 && currentTargetIndex + 1 < path.points.size()) {

			if (currentTarget.x == 0 * CELL_SIZE + CELL_SIZE / 2 && currentTarget.y == 10 * CELL_SIZE + CELL_SIZE / 2 && path.points[currentTargetIndex + 1].x == 39 * CELL_SIZE + CELL_SIZE / 2 && path.points[currentTargetIndex + 1].y == 10 * CELL_SIZE + CELL_SIZE / 2) {
				agents[0]->setPosition(Vector2D(39 * CELL_SIZE + CELL_SIZE / 2, 10 * CELL_SIZE + CELL_SIZE / 2));
				currentTargetIndex++;
				currentTarget = path.points[currentTargetIndex];
			}
			else if (currentTarget.x == 39 * CELL_SIZE + CELL_SIZE / 2 && currentTarget.y == 10 * CELL_SIZE + CELL_SIZE / 2 && path.points[currentTargetIndex + 1].x == 0 * CELL_SIZE + CELL_SIZE / 2 && path.points[currentTargetIndex + 1].y == 10 * CELL_SIZE + CELL_SIZE / 2) {
				agents[0]->setPosition(Vector2D(0 * CELL_SIZE + CELL_SIZE / 2, 10 * CELL_SIZE + CELL_SIZE / 2));
				currentTargetIndex++;
				currentTarget = path.points[currentTargetIndex];
			}
			else if (currentTarget.x == 0 * CELL_SIZE + CELL_SIZE / 2 && currentTarget.y == 11 * CELL_SIZE + CELL_SIZE / 2 && path.points[currentTargetIndex + 1].x == 39 * CELL_SIZE + CELL_SIZE / 2 && path.points[currentTargetIndex + 1].y == 11 * CELL_SIZE + CELL_SIZE / 2) {
				agents[0]->setPosition(Vector2D(39 * CELL_SIZE + CELL_SIZE / 2, 11 * CELL_SIZE + CELL_SIZE / 2));
				currentTargetIndex++;
				currentTarget = path.points[currentTargetIndex];
			}
			else if (currentTarget.x == 39 * CELL_SIZE + CELL_SIZE / 2 && currentTarget.y == 11 * CELL_SIZE + CELL_SIZE / 2 && path.points[currentTargetIndex + 1].x == 0 * CELL_SIZE + CELL_SIZE / 2 && path.points[currentTargetIndex + 1].y == 11 * CELL_SIZE + CELL_SIZE / 2) {
				agents[0]->setPosition(Vector2D(0 * CELL_SIZE + CELL_SIZE / 2, 11 * CELL_SIZE + CELL_SIZE / 2));
				currentTargetIndex++;
				currentTarget = path.points[currentTargetIndex];
			}
			else if (currentTarget.x == 0 * CELL_SIZE + CELL_SIZE / 2 && currentTarget.y == 12 * CELL_SIZE + CELL_SIZE / 2 && path.points[currentTargetIndex + 1].x == 39 * CELL_SIZE + CELL_SIZE / 2 && path.points[currentTargetIndex + 1].y == 12 * CELL_SIZE + CELL_SIZE / 2) {
				agents[0]->setPosition(Vector2D(39 * CELL_SIZE + CELL_SIZE / 2, 12 * CELL_SIZE + CELL_SIZE / 2));
				currentTargetIndex++;
				currentTarget = path.points[currentTargetIndex];
			}
			else if (currentTarget.x == 39 * CELL_SIZE + CELL_SIZE / 2 && currentTarget.y == 12 * CELL_SIZE + CELL_SIZE / 2 && path.points[currentTargetIndex + 1].x == 0 * CELL_SIZE + CELL_SIZE / 2 && path.points[currentTargetIndex + 1].y == 12 * CELL_SIZE + CELL_SIZE / 2) {
				agents[0]->setPosition(Vector2D(0 * CELL_SIZE + CELL_SIZE / 2, 12 * CELL_SIZE + CELL_SIZE / 2));
				currentTargetIndex++;
				currentTarget = path.points[currentTargetIndex];
			}
		}


	}
	else
	{
		agents[0]->update(Vector2D(0, 0), dtime, event);
	}

}

void Scene_WaypointPathFinding::draw()
{
	drawFrontier();
	drawTerrains();
	drawMaze();
	drawCoin();


	if (draw_grid)
	{
		SDL_SetRenderDrawColor(TheApp::Instance()->getRenderer(), 255, 255, 255, 127);
		for (int i = 0; i < SRC_WIDTH; i += CELL_SIZE)
		{
			SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), i, 0, i, SRC_HEIGHT);
		}
		for (int j = 0; j < SRC_HEIGHT; j = j += CELL_SIZE)
		{
			SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), 0, j, SRC_WIDTH, j);
		}
	}

	for (int i = 0; i < (int)path.points.size(); i++)
	{

		draw_circle(TheApp::Instance()->getRenderer(), (int)(path.points[i].x), (int)(path.points[i].y), 15, 255, 255, 0, 255);
		//if (i > 0) {
		//	SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), (int)(path.points[i - 1].x), (int)(path.points[i - 1].y), (int)(path.points[i].x), (int)(path.points[i].y));
		//}
	}

	draw_circle(TheApp::Instance()->getRenderer(), (int)currentTarget.x, (int)currentTarget.y, 15, 255, 0, 0, 255);

	agents[0]->draw();
}

const char* Scene_WaypointPathFinding::getTitle()
{
	return "SDL Steering Behaviors :: PathFinding1 Demo";
}

void Scene_WaypointPathFinding::drawMaze()
{
	if (draw_grid)
	{

		SDL_SetRenderDrawColor(TheApp::Instance()->getRenderer(), 0, 0, 255, 255);
		for (unsigned int i = 0; i < maze_rects.size(); i++)
			SDL_RenderFillRect(TheApp::Instance()->getRenderer(), &maze_rects[i]);
	}
	else
	{
		SDL_RenderCopy(TheApp::Instance()->getRenderer(), background_texture, NULL, NULL);
	}
}

void Scene_WaypointPathFinding::drawFrontier() {
	if (draw_grid)
	{
		frontier_rects = agents[0]->pathFinder->returnFrontier();
		SDL_SetRenderDrawColor(TheApp::Instance()->getRenderer(), 155, 155, 155, 255);
		for (unsigned int i = 0; i < frontier_rects.size(); i++)
			SDL_RenderFillRect(TheApp::Instance()->getRenderer(), &frontier_rects[i]);
	}
}

void Scene_WaypointPathFinding::drawTerrains() {
	if (draw_grid)
	{
		for (unsigned int i = 0; i < terrains.size(); i++) {
			if (terrains[i].second > 400) {
				SDL_SetRenderDrawColor(TheApp::Instance()->getRenderer(), 255, 0, 0, 255);
			}
			else if (terrains[i].second > 3) {
				SDL_SetRenderDrawColor(TheApp::Instance()->getRenderer(), 100, 0, 0, 255);
			}
			else if (terrains[i].second > 1) {
				SDL_SetRenderDrawColor(TheApp::Instance()->getRenderer(), 50, 0, 0, 255);
			}
			SDL_RenderFillRect(TheApp::Instance()->getRenderer(), &terrains[i].first);
		}
	}
}

void Scene_WaypointPathFinding::drawCoin()
{
	for each (Vector2D* c in coinsPositions)
	{
		Vector2D coin_coords = cell2pix(*c);
		int offset = CELL_SIZE / 2;
		SDL_Rect dstrect = { (int)coin_coords.x - offset, (int)coin_coords.y - offset, CELL_SIZE, CELL_SIZE };
		SDL_RenderCopy(TheApp::Instance()->getRenderer(), coin_texture, NULL, &dstrect);
	}

}

void Scene_WaypointPathFinding::initTerrains() {

	float cost1 = 500.f;
	float cost2 = 2.f;
	float cost3 = 5.f;

	SDL_Rect rect = { 18 * CELL_SIZE, 9 * CELL_SIZE, CELL_SIZE,  CELL_SIZE };
	terrains.push_back(std::pair<SDL_Rect, float>(rect, cost2));
	rect = { 18 * CELL_SIZE, 14 * CELL_SIZE, CELL_SIZE,  CELL_SIZE };
	terrains.push_back(std::pair<SDL_Rect, float>(rect, cost1));
	rect = { 18 * CELL_SIZE, 13 * CELL_SIZE, CELL_SIZE,  CELL_SIZE };
	terrains.push_back(std::pair<SDL_Rect, float>(rect, cost1));
	rect = { 18 * CELL_SIZE, 12 * CELL_SIZE, CELL_SIZE,  CELL_SIZE };
	terrains.push_back(std::pair<SDL_Rect, float>(rect, cost1));
	rect = { 18 * CELL_SIZE, 11 * CELL_SIZE, CELL_SIZE,  CELL_SIZE };
	terrains.push_back(std::pair<SDL_Rect, float>(rect, cost1));
	rect = { 18 * CELL_SIZE, 10 * CELL_SIZE, CELL_SIZE,  CELL_SIZE };
	terrains.push_back(std::pair<SDL_Rect, float>(rect, cost1));
	rect = { 19 * CELL_SIZE, 9 * CELL_SIZE, CELL_SIZE,  CELL_SIZE };
	terrains.push_back(std::pair<SDL_Rect, float>(rect, cost2));
	rect = { 19 * CELL_SIZE, 14 * CELL_SIZE, CELL_SIZE,  CELL_SIZE };
	terrains.push_back(std::pair<SDL_Rect, float>(rect, cost1));
	rect = { 19 * CELL_SIZE, 13 * CELL_SIZE, CELL_SIZE,  CELL_SIZE };
	terrains.push_back(std::pair<SDL_Rect, float>(rect, cost1));
	rect = { 19 * CELL_SIZE, 12 * CELL_SIZE, CELL_SIZE,  CELL_SIZE };
	terrains.push_back(std::pair<SDL_Rect, float>(rect, cost1));
	rect = { 19 * CELL_SIZE, 11 * CELL_SIZE, CELL_SIZE,  CELL_SIZE };
	terrains.push_back(std::pair<SDL_Rect, float>(rect, cost1));
	rect = { 19 * CELL_SIZE, 10 * CELL_SIZE, CELL_SIZE,  CELL_SIZE };
	terrains.push_back(std::pair<SDL_Rect, float>(rect, cost1));
	rect = { 20 * CELL_SIZE, 9 * CELL_SIZE, CELL_SIZE,  CELL_SIZE };
	terrains.push_back(std::pair<SDL_Rect, float>(rect, cost2));
	rect = { 20 * CELL_SIZE, 14 * CELL_SIZE, CELL_SIZE,  CELL_SIZE };
	terrains.push_back(std::pair<SDL_Rect, float>(rect, cost1));
	rect = { 20 * CELL_SIZE, 13 * CELL_SIZE, CELL_SIZE,  CELL_SIZE };
	terrains.push_back(std::pair<SDL_Rect, float>(rect, cost1));
	rect = { 20 * CELL_SIZE, 12 * CELL_SIZE, CELL_SIZE,  CELL_SIZE };
	terrains.push_back(std::pair<SDL_Rect, float>(rect, cost1));
	rect = { 20 * CELL_SIZE, 11 * CELL_SIZE, CELL_SIZE,  CELL_SIZE };
	terrains.push_back(std::pair<SDL_Rect, float>(rect, cost1));
	rect = { 20 * CELL_SIZE, 10 * CELL_SIZE, CELL_SIZE,  CELL_SIZE };
	terrains.push_back(std::pair<SDL_Rect, float>(rect, cost1));
	rect = { 21 * CELL_SIZE, 9 * CELL_SIZE, CELL_SIZE,  CELL_SIZE };
	terrains.push_back(std::pair<SDL_Rect, float>(rect, cost2));
	rect = { 21 * CELL_SIZE, 10 * CELL_SIZE, CELL_SIZE,  CELL_SIZE };
	terrains.push_back(std::pair<SDL_Rect, float>(rect, cost1));
	rect = { 21 * CELL_SIZE, 11 * CELL_SIZE, CELL_SIZE,  CELL_SIZE };
	terrains.push_back(std::pair<SDL_Rect, float>(rect, cost1));
	rect = { 21 * CELL_SIZE, 12 * CELL_SIZE, CELL_SIZE,  CELL_SIZE };
	terrains.push_back(std::pair<SDL_Rect, float>(rect, cost1));
	rect = { 21 * CELL_SIZE, 13 * CELL_SIZE, CELL_SIZE,  CELL_SIZE };
	terrains.push_back(std::pair<SDL_Rect, float>(rect, cost1));
	rect = { 21 * CELL_SIZE, 14 * CELL_SIZE, CELL_SIZE,  CELL_SIZE };
	terrains.push_back(std::pair<SDL_Rect, float>(rect, cost1));
	rect = { 21 * CELL_SIZE, 15 * CELL_SIZE, CELL_SIZE,  CELL_SIZE };
	terrains.push_back(std::pair<SDL_Rect, float>(rect, cost3));
	rect = { 18 * CELL_SIZE, 15 * CELL_SIZE, CELL_SIZE,  CELL_SIZE };
	terrains.push_back(std::pair<SDL_Rect, float>(rect, cost3));
	rect = { 19 * CELL_SIZE, 15 * CELL_SIZE, CELL_SIZE,  CELL_SIZE };
	terrains.push_back(std::pair<SDL_Rect, float>(rect, cost3));
	rect = { 20 * CELL_SIZE, 15 * CELL_SIZE, CELL_SIZE,  CELL_SIZE };
	terrains.push_back(std::pair<SDL_Rect, float>(rect, cost3));
	rect = { 21 * CELL_SIZE, 15 * CELL_SIZE, CELL_SIZE,  CELL_SIZE };
	terrains.push_back(std::pair<SDL_Rect, float>(rect, cost3));
	rect = { 18 * CELL_SIZE, 14 * CELL_SIZE, CELL_SIZE,  CELL_SIZE };
	terrains.push_back(std::pair<SDL_Rect, float>(rect, cost1));
	rect = { 18 * CELL_SIZE, 13 * CELL_SIZE, CELL_SIZE,  CELL_SIZE };
	terrains.push_back(std::pair<SDL_Rect, float>(rect, cost1));
	rect = { 18 * CELL_SIZE, 12 * CELL_SIZE, CELL_SIZE,  CELL_SIZE };
	terrains.push_back(std::pair<SDL_Rect, float>(rect, cost1));
	rect = { 18 * CELL_SIZE, 11 * CELL_SIZE, CELL_SIZE,  CELL_SIZE };
	terrains.push_back(std::pair<SDL_Rect, float>(rect, cost1));
	rect = { 18 * CELL_SIZE, 10 * CELL_SIZE, CELL_SIZE,  CELL_SIZE };
	terrains.push_back(std::pair<SDL_Rect, float>(rect, cost1));
	
	

}

void Scene_WaypointPathFinding::initMaze()
{

	// Initialize a list of Rectagles describing the maze geometry (useful for collision avoidance)
	SDL_Rect rect = { 0, 0, 1280, 32 };
	maze_rects.push_back(rect);
	rect = { 608, 32, 64, 32 };
	maze_rects.push_back(rect);
	rect = { 0, 736, 1280, 32 };
	maze_rects.push_back(rect);
	rect = { 608, 512, 64, 224 };
	maze_rects.push_back(rect);
	rect = { 0,32,32,288 };
	maze_rects.push_back(rect);
	rect = { 0,416,32,320 };
	maze_rects.push_back(rect);
	rect = { 1248,32,32,288 };
	maze_rects.push_back(rect);
	rect = { 1248,416,32,320 };
	maze_rects.push_back(rect);
	rect = { 128,128,64,32 };
	maze_rects.push_back(rect);
	rect = { 288,128,96,32 };
	maze_rects.push_back(rect);
	rect = { 480,128,64,32 };
	maze_rects.push_back(rect);
	rect = { 736,128,64,32 };
	maze_rects.push_back(rect);
	rect = { 896,128,96,32 };
	maze_rects.push_back(rect);
	rect = { 1088,128,64,32 };
	maze_rects.push_back(rect);
	rect = { 128,256,64,32 };
	maze_rects.push_back(rect);
	rect = { 288,256,96,32 };
	maze_rects.push_back(rect);
	rect = { 480, 256, 320, 32 };
	maze_rects.push_back(rect);
	rect = { 608, 224, 64, 32 };
	maze_rects.push_back(rect);
	rect = { 896,256,96,32 };
	maze_rects.push_back(rect);
	rect = { 1088,256,64,32 };
	maze_rects.push_back(rect);
	rect = { 128,384,32,256 };
	maze_rects.push_back(rect);
	rect = { 160,512,352,32 };
	maze_rects.push_back(rect);
	rect = { 1120,384,32,256 };
	maze_rects.push_back(rect);
	rect = { 768,512,352,32 };
	maze_rects.push_back(rect);
	rect = { 256,640,32,96 };
	maze_rects.push_back(rect);
	rect = { 992,640,32,96 };
	maze_rects.push_back(rect);
	rect = { 384,544,32,96 };
	maze_rects.push_back(rect);
	rect = { 480,704,32,32 };
	maze_rects.push_back(rect);
	rect = { 768,704,32,32 };
	maze_rects.push_back(rect);
	rect = { 864,544,32,96 };
	maze_rects.push_back(rect);
	rect = { 320,288,32,128 };
	maze_rects.push_back(rect);
	rect = { 352,384,224,32 };
	maze_rects.push_back(rect);
	rect = { 704,384,224,32 };
	maze_rects.push_back(rect);
	rect = { 928,288,32,128 };
	maze_rects.push_back(rect);

	// Initialize the terrain matrix (for each cell a zero value indicates it's a wall)

	// (1st) initialize all cells to 1 by default
	for (int i = 0; i < num_cell_x; i++)
	{
		vector<int> terrain_col(num_cell_y, 1);
		terrain.push_back(terrain_col);
	}
	// (2nd) set to zero all cells that belong to a wall
	int offset = CELL_SIZE / 2;

	for (int i = 0; i < num_cell_x; i++)
	{
		for (int j = 0; j < num_cell_y; j++)
		{
			Vector2D cell_center((float)(i*CELL_SIZE + offset), (float)(j*CELL_SIZE + offset));
			for (unsigned int b = 0; b < maze_rects.size(); b++)
			{
				if (Vector2DUtils::IsInsideRect(cell_center, (float)maze_rects[b].x, (float)maze_rects[b].y, (float)maze_rects[b].w, (float)maze_rects[b].h))
				{
					terrain[i][j] = 0;
					break;
				}

			}

		}
	}

	for (int i = 0; i < terrains.size(); i++) {

		terrain[terrains[i].first.x / CELL_SIZE][terrains[i].first.y / CELL_SIZE] = terrains[i].second;

	}

	for (int i = 0; i < num_cell_x; i++)
	{
		for (int j = 0; j < num_cell_y; j++)
		{
			if (terrain[i][j] > 0) {
				//Left node
				if (i - 1 >= 0 && terrain[i - 1][j] > 0) {
					terrainGraph.addConnection(new Vector2D((float)(i*CELL_SIZE + offset), (float)(j*CELL_SIZE + offset)), new Vector2D((float)((i - 1)*CELL_SIZE + offset), (float)(j*CELL_SIZE + offset)), i, j, terrain[i - 1][j]);
					//std::cout << "From Position x: " << i << " Position y: " << j << " To Position x: " << i - 1 << " Position y: " << j << endl;
				}
				//Right node
				if (i + 1 < terrain.size() && terrain[i + 1][j] > 0) {
					terrainGraph.addConnection(new Vector2D((float)(i*CELL_SIZE + offset), (float)(j*CELL_SIZE + offset)), new Vector2D((float)((i + 1)*CELL_SIZE + offset), (float)(j*CELL_SIZE + offset)), i, j, terrain[i + 1][j]);
					//std::cout << "From Position x: " << i << " Position y: " << j << " To Position x: " << i + 1 << " Position y: " << j << endl;
				}
				//Up node
				if (j - 1 >= 0 && terrain[i][j - 1] > 0) {
					terrainGraph.addConnection(new Vector2D((float)(i*CELL_SIZE + offset), (float)(j*CELL_SIZE + offset)), new Vector2D((float)((i)*CELL_SIZE + offset), (float)((j - 1)*CELL_SIZE + offset)), i, j, terrain[i][j - 1]);
					//std::cout << "From Position x: " << i << " Position y: " << j << " To Position x: " << i << " Position y: " << j - 1 << endl;
				}
				//Down node
				if (j + 1 < terrain[i].size() && terrain[i][j + 1] > 0) {
					terrainGraph.addConnection(new Vector2D((float)(i*CELL_SIZE + offset), (float)(j*CELL_SIZE + offset)), new Vector2D((float)((i)*CELL_SIZE + offset), (float)((j + 1)*CELL_SIZE + offset)), i, j, terrain[i][j + 1]);
					//std::cout << "From Position x: " << i << " Position y: " << j << " To Position x: " << i - 1 << " Position y: " << j + 1 << endl;
				}

			}
			
		}
		

		terrainGraph.addConnection(new Vector2D((float)(0 * CELL_SIZE + offset), (float)(10 * CELL_SIZE + offset)), new Vector2D((float)(39 * CELL_SIZE + offset), (float)(10 * CELL_SIZE + offset)), 0, 10, 1);
		terrainGraph.addConnection(new Vector2D((float)(0 * CELL_SIZE + offset), (float)(11 * CELL_SIZE + offset)), new Vector2D((float)(39 * CELL_SIZE + offset), (float)(11 * CELL_SIZE + offset)), 0, 11, 1);
		terrainGraph.addConnection(new Vector2D((float)(0 * CELL_SIZE + offset), (float)(12 * CELL_SIZE + offset)), new Vector2D((float)(39 * CELL_SIZE + offset), (float)(12 * CELL_SIZE + offset)), 0, 12, 1);

		terrainGraph.addConnection(new Vector2D((float)(39 * CELL_SIZE + offset), (float)(10 * CELL_SIZE + offset)), new Vector2D((float)(0 * CELL_SIZE + offset), (float)(10 * CELL_SIZE + offset)), 39, 10, 1);
		terrainGraph.addConnection(new Vector2D((float)(39 * CELL_SIZE + offset), (float)(11 * CELL_SIZE + offset)), new Vector2D((float)(0 * CELL_SIZE + offset), (float)(11 * CELL_SIZE + offset)), 39, 11, 1);
		terrainGraph.addConnection(new Vector2D((float)(39 * CELL_SIZE + offset), (float)(12 * CELL_SIZE + offset)), new Vector2D((float)(0 * CELL_SIZE + offset), (float)(12 * CELL_SIZE + offset)), 39, 12, 1);
	}

}

bool Scene_WaypointPathFinding::loadTextures(char* filename_bg, char* filename_coin)
{
	SDL_Surface *image = IMG_Load(filename_bg);
	if (!image) {
		cout << "IMG_Load: " << IMG_GetError() << endl;
		return false;
	}
	background_texture = SDL_CreateTextureFromSurface(TheApp::Instance()->getRenderer(), image);

	if (image)
		SDL_FreeSurface(image);

	image = IMG_Load(filename_coin);
	if (!image) {
		cout << "IMG_Load: " << IMG_GetError() << endl;
		return false;
	}
	coin_texture = SDL_CreateTextureFromSurface(TheApp::Instance()->getRenderer(), image);

	if (image)
		SDL_FreeSurface(image);

	return true;
}

Vector2D Scene_WaypointPathFinding::cell2pix(Vector2D cell)
{
	int offset = CELL_SIZE / 2;
	return Vector2D(cell.x*CELL_SIZE + offset, cell.y*CELL_SIZE + offset);
}

Vector2D Scene_WaypointPathFinding::pix2cell(Vector2D pix)
{
	return Vector2D((float)((int)pix.x / CELL_SIZE), (float)((int)pix.y / CELL_SIZE));
}

bool Scene_WaypointPathFinding::isValidCell(Vector2D cell)
{
	if ((cell.x < 0) || (cell.y < 0) || (cell.x >= terrain.size()) || (cell.y >= terrain[0].size()))
		return false;
	return !(terrain[(unsigned int)cell.x][(unsigned int)cell.y] == 0);
}