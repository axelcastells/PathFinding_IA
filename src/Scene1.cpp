#include "Scene1.h"

using namespace std;

#define MAX_COINS 3

Scene1::Scene1()
{
	draw_grid = false;

	num_cell_x = SRC_WIDTH / CELL_SIZE;
	num_cell_y = SRC_HEIGHT / CELL_SIZE;
	terrainGraph = Graph();
	initMaze();
	loadTextures("../res/maze.png", "../res/coin.png");
	srand((unsigned int)time(NULL));
	Agent *agent = new Agent;
	agent->loadSpriteTexture("../res/soldier.png", 4);
	agents.push_back(agent);

	currentCoinIndex = 0;

	// set agent position coords to the center of a random cell
	Vector2D rand_cell(-1, -1);
	while (!isValidCell(rand_cell))
		rand_cell = Vector2D((float)(rand() % num_cell_x), (float)(rand() % num_cell_y));
	agents[0]->setPosition(cell2pix(rand_cell));
	agents[0]->SetPathFinderGraph(&path, &terrainGraph);

	// set the coin in a random cell (but at least 3 cells far from the agent)
	for (int i = 0; i < MAX_COINS; i++)
	{
		coinPosition.push_back(Vector2D(-1, -1));
		//Vector2D tempCoin = Vector2D(-1, -1);
		while ((!isValidCell(coinPosition[i])) || (Vector2D::Distance(coinPosition[i], rand_cell)<3))
			coinPosition[i] = (Vector2D((float)(rand() % num_cell_x), (float)(rand() % num_cell_y)));
	}


	// PathFollowing next Target
	currentTarget = Vector2D(0, 0);
	currentTargetIndex = -1;
	agents[0]->searchActive = true;

	waypointCount = MAX_COINS;

}

Scene1::~Scene1()
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

void Scene1::update(float dtime, SDL_Event *event)
{
	/* Keyboard & Mouse events */
	
	switch (event->type) {
	case SDL_KEYDOWN:
		if (event->key.keysym.scancode == SDL_SCANCODE_SPACE)
			draw_grid = !draw_grid;

		break;
	case SDL_MOUSEMOTION:
	/*case SDL_MOUSEBUTTONDOWN:
		if (event->button.button == SDL_BUTTON_LEFT)
		{
			Vector2D cell = pix2cell(Vector2D((float)(event->button.x), (float)(event->button.y)));
			if (isValidCell(cell))
			{
				if (path.points.size() > 0)
					if (path.points[path.points.size() - 1] == cell2pix(cell))
						break;

				path.points.push_back(cell2pix(cell));
			}
		}
		break;*/
	default:
		break;
	}


	agents[0]->setTarget(cell2pix(coinPosition[currentCoinIndex]));

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
					// if we have arrived to the coin, replace it ina random cell!
					
					if (pix2cell(agents[0]->getPosition()) == coinPosition[currentCoinIndex])
					{
						currentCoinIndex++;
						//---
						//coinPosition[currentCoinIndex] = Vector2D(-1, -1);
						//Indicam que el nou path no ha estat trobat
						agents[0]->pathFinder->pathFound = false;

						if (currentCoinIndex >= coinPosition.size())
						{
							for (int i = 0; i < coinPosition.size(); i++)
							{
								//coinPosition[currentCoinIndex] = Vector2D(-1, -1);
								//Indicam que el nou path no ha estat trobat
								agents[0]->pathFinder->pathFound = false;
								//coinPosition.push_back(Vector2D(-1, -1));
								//Vector2D tempCoin = Vector2D(-1, -1);
								while ((!isValidCell(coinPosition[i])) || (Vector2D::Distance(coinPosition[i], pix2cell(agents[0]->getPosition()))<3))
									coinPosition[i] = Vector2D((float)(rand() % num_cell_x), (float)(rand() % num_cell_y));
								
							}

							currentCoinIndex = 0;
						}

					}
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
		//if (currentCoinIndex < coinPosition.size())
		//{
		//	currentCoinIndex++;
		//	agents[0]->setTarget(cell2pix(coinPosition[currentCoinIndex]));
		//	currentTargetIndex = 0;
		//}
	}
	
}

void Scene1::draw()
{
	drawMaze();

	for each (Vector2D c in coinPosition)
	{
		drawCoin(c);
	}
	//drawCoin();
	

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

		if (i > 0) {
			//SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), (int)(path.points[i - 1].x), (int)(path.points[i - 1].y), (int)(path.points[i].x), (int)(path.points[i].y));
		}			
	}

	draw_circle(TheApp::Instance()->getRenderer(), (int)currentTarget.x, (int)currentTarget.y, 15, 255, 0, 0, 255);

	agents[0]->draw();
}

const char* Scene1::getTitle()
{
	return "SDL Steering Behaviors :: PathFinding1 Demo";
}

void Scene1::drawMaze()
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

void Scene1::drawCoin(Vector2D pos)
{
	Vector2D coin_coords = cell2pix(pos);
	int offset = CELL_SIZE / 2;
	SDL_Rect dstrect = { (int)coin_coords.x - offset, (int)coin_coords.y - offset, CELL_SIZE, CELL_SIZE };
	SDL_RenderCopy(TheApp::Instance()->getRenderer(), coin_texture, NULL, &dstrect);
}

void Scene1::initMaze()
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

	for (int i = 0; i < num_cell_x; i++)
	{
		for (int j = 0; j < num_cell_y; j++)
		{
			if (terrain[i][j] == 1) {
				//Left node
				if (i - 1 >= 0 && terrain[i - 1][j] == 1) {
					terrainGraph.addConnection(new Vector2D((float)(i*CELL_SIZE + offset), (float)(j*CELL_SIZE + offset)), new Vector2D((float)((i - 1)*CELL_SIZE + offset), (float)(j*CELL_SIZE + offset)), i, j, 1);
					//std::cout << "From Position x: " << i << " Position y: " << j << " To Position x: " << i - 1 << " Position y: " << j << endl;
				}
				//Right node
				if (i + 1 < terrain.size() && terrain[i + 1][j] == 1) {
					terrainGraph.addConnection(new Vector2D((float)(i*CELL_SIZE + offset), (float)(j*CELL_SIZE + offset)), new Vector2D((float)((i + 1)*CELL_SIZE + offset), (float)(j*CELL_SIZE + offset)), i, j, 1);
					//std::cout << "From Position x: " << i << " Position y: " << j << " To Position x: " << i + 1 << " Position y: " << j << endl;
				}
				//Up node
				if (j - 1 >= 0 && terrain[i][j-1] == 1) {
					terrainGraph.addConnection(new Vector2D((float)(i*CELL_SIZE + offset), (float)(j*CELL_SIZE + offset)), new Vector2D((float)((i)*CELL_SIZE + offset), (float)((j - 1)*CELL_SIZE + offset)), i, j, 1);
					//std::cout << "From Position x: " << i << " Position y: " << j << " To Position x: " << i << " Position y: " << j - 1 << endl;
				}
				//Down node
				if (j + 1 < terrain[i].size() && terrain[i][j+1] == 1) {
					terrainGraph.addConnection(new Vector2D((float)(i*CELL_SIZE + offset), (float)(j*CELL_SIZE + offset)), new Vector2D((float)((i)*CELL_SIZE + offset), (float)((j + 1)*CELL_SIZE + offset)), i, j, 1);
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

	//vector<pair<Vector2D*, float>> tmp = terrainGraph.getNextNodes(0,0);
	/*for (int i = 0; i < tmp.size(); ++i) {
		cout << "Possible next node: " << i + 1 << " X: " << (int)tmp[i].first->x / CELL_SIZE << " Y: " << (int)tmp[i].first->y / CELL_SIZE << " with cost: " << tmp[i].second << endl;
	}*/

}

bool Scene1::loadTextures(char* filename_bg, char* filename_coin)
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

Vector2D Scene1::cell2pix(Vector2D cell)
{
	int offset = CELL_SIZE / 2;
	return Vector2D(cell.x*CELL_SIZE + offset, cell.y*CELL_SIZE + offset);
}

Vector2D Scene1::pix2cell(Vector2D pix)
{
	return Vector2D((float)((int)pix.x / CELL_SIZE), (float)((int)pix.y / CELL_SIZE));
}

bool Scene1::isValidCell(Vector2D cell)
{
	if ((cell.x < 0) || (cell.y < 0) || (cell.x >= terrain.size()) || (cell.y >= terrain[0].size()))
		return false;
	return !(terrain[(unsigned int)cell.x][(unsigned int)cell.y] == 0);
}