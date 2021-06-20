#include <SFML/Graphics.hpp>
#include"pathNode.h"
#include<queue>
#include<list>
#include<iostream>

const int gridHeight = 80;
const int gridWidth = 80;
const int cellSize = 10;
using namespace std;

struct sortByFCost {
	bool operator()(const pathNode* p1, const pathNode* p2) const {
		return (*p1).fCost > (*p2).fCost;
	}
};

void setUpGrid(pathNode* grid) {
	for (int x = 0; x < gridWidth; x++)
	{
		for (int y = 0; y < gridHeight; y++)
		{

			grid[y * gridWidth + x].position = sf::Vector2i(x, y);
			grid[y * gridWidth + x].isObstacle = false;
			grid[y * gridWidth + x].parent = nullptr;
			grid[y * gridWidth + x].isVisited = false;
		}
	}

	for (int x = 0; x < gridWidth; x++)
	{
		for (int y = 0; y < gridHeight; y++)
		{

			if (y > 0)
				grid[y * gridWidth + x].neighbours.push_back(&grid[(y - 1) * gridWidth + (x + 0)]);
			if (y < gridHeight - 1)
				grid[y * gridWidth + x].neighbours.push_back(&grid[(y + 1) * gridWidth + (x + 0)]);
			if (x > 0)
				grid[y * gridWidth + x].neighbours.push_back(&grid[(y + 0) * gridWidth + (x - 1)]);
			if (x < gridWidth - 1)
				grid[y * gridWidth + x].neighbours.push_back(&grid[(y + 0) * gridWidth + (x + 1)]);
			/*
			if (y>0 && x>0)
				grid[y* gridWidth + x].neighbours.push_back(&grid[(y - 1) * gridWidth + (x - 1)]);
			if (y< gridHeight -1 && x>0)
					grid[y* gridWidth + x].neighbours.push_back(&grid[(y + 1) * gridWidth + (x - 1)]);
			if (y>0 && x< gridWidth -1)
					grid[y* gridWidth + x].neighbours.push_back(&grid[(y - 1) * gridWidth + (x + 1)]);
			if (y< gridHeight - 1 && x< gridWidth -1)
					grid[y* gridWidth + x].neighbours.push_back(&grid[(y + 1) * gridWidth + (x + 1)]);
				*/


		}
	}
}
void drawGrid(sf::RenderWindow& window, pathNode* grid) {
	for (int x = 0; x < gridWidth; x++)
	{
		for (int y = 0; y < gridHeight; y++)
		{
			if (grid[y * gridWidth + x].isObstacle || grid[y * gridWidth + x].isVisited) {
				//grid[y * gridWidth + x].position = sf::Vector2i(x, y);
				sf::RectangleShape rectangle(sf::Vector2f(cellSize, cellSize));
				sf::Vector2i pos = grid[y * gridWidth + x].position;
				rectangle.setPosition(pos.x * cellSize, (pos.y * cellSize));

				if (grid[y * gridWidth + x].isVisited) {
					rectangle.setFillColor(sf::Color(169, 169, 169));;
					//continue;
				}
				if (grid[y * gridWidth + x].isObstacle) {
					rectangle.setFillColor(sf::Color::Blue);
				}

				window.draw(rectangle);
			}

			
		}
	}
}

float distance(sf::Vector2i p1, sf::Vector2i p2) {
	float xDif = p1.x - p2.x;
	float yDif = p1.y - p2.y;
	return sqrtf((xDif * xDif) + (yDif * yDif));
}
bool SolveAstar(pathNode* grid,pathNode* startN,pathNode* endN) {
	for (int x = 0; x < gridWidth; x++)
	{
		for (int y = 0; y < gridHeight; y++)
		{

			grid[y * gridWidth + x].isVisited = false;
			grid[y * gridWidth + x].parent = nullptr;
			grid[y * gridWidth + x].gCost = INFINITY;
			grid[y * gridWidth + x].hCost = INFINITY;

		}
	}
	(*startN).gCost = 0;
	(*startN).hCost = distance((*startN).position,(*endN).position);
	(*startN).fCost = distance((*startN).position,(*endN).position);
	//priority_queue<pathNode*,vector<pathNode*>, sortByFCost> notTestedList;
	list<pathNode*> notTestedList;

	notTestedList.push_back(startN);
	pathNode* current = startN;
	while (!notTestedList.empty() && current != endN)// Find absolutely shortest path // && nodeCurrent != nodeEnd)
	{
		notTestedList.sort([](const pathNode* lhs, const pathNode* rhs) { return lhs->fCost < rhs->fCost; });

		while (!notTestedList.empty() && (*notTestedList.front()).isVisited)
			notTestedList.pop_front();

		// ...or abort because there are no valid nodes left to test
		if (notTestedList.empty())
			break;

		current = notTestedList.front();
		current->isVisited = true;
		for (auto nodeNeighbour : current->neighbours)
		{
			//nodeNeighbour->gCost = distance(startN->position, nodeNeighbour->position);
			//nodeNeighbour->hCost = distance(endN->position, nodeNeighbour->position);
			//nodeNeighbour->calculateFCost();

			if (!nodeNeighbour->isVisited && nodeNeighbour->isObstacle==0) {
				notTestedList.push_back(nodeNeighbour);
			}
			float tentative_gScore = current->gCost + distance(current->position, nodeNeighbour->position);
			if (tentative_gScore < nodeNeighbour->gCost) {
				nodeNeighbour->parent = current;
				nodeNeighbour->gCost = tentative_gScore;
				nodeNeighbour->fCost = tentative_gScore + distance(nodeNeighbour->position, endN->position);
				//cout << "[" << nodeNeighbour->position.x << "," << nodeNeighbour->position.y << "]-\t" << nodeNeighbour->parent->position.x << "," << nodeNeighbour->parent->position.y << endl;

			}

		}
	}
	/*
		pathNode* helper = endN;
		while (helper->parent != nullptr) {
			
			cout << "[" << helper->position.x << "," << helper->position.y << "]-\t" << helper->parent->position.x << "," << helper->parent->position.x << endl;
			cout << helper->gCost << "\t" << helper->parent->gCost << endl;
		}

	*/


	return true;



	//list<sNode*> listNotTestedNodes;


}
void drawWall(pathNode* grid, sf::Vector2i gridPositions) {
	grid[gridPositions.y * gridWidth + gridPositions.x].isObstacle = true;

}

int main()
{

	sf::RenderWindow window(sf::VideoMode(800, 800), "Hello, SFML world!");
	window.setFramerateLimit(60);
	bool rightMouseButtonHolding = false;

	pathNode grid[gridHeight * gridWidth];
	setUpGrid(grid);
	pathNode* startN = &grid[1 * gridWidth + 2];
	pathNode* endN = &grid[10 * gridWidth + 50];

	bool startOrEndPositionChange = true;
	SolveAstar(grid,startN,endN);


	//grid[5 * gridWidth + 5].isObstacle = true;
	//grid[7 * gridWidth + 7].isVisited = true;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::MouseButtonPressed)
			{
				sf::Vector2i localPosition = sf::Mouse::getPosition(window);
				sf::Vector2i gridPositions(localPosition.x / cellSize, localPosition.y  / cellSize);

				if (event.mouseButton.button == sf::Mouse::Right)
				{
					rightMouseButtonHolding = true;
					drawWall(grid,gridPositions);
					SolveAstar(grid, startN, endN);
				}
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					if (startOrEndPositionChange) {
						startN = &grid[gridPositions.y * gridWidth + gridPositions.x];
						startN->isObstacle = false;

						startOrEndPositionChange = !startOrEndPositionChange;
						SolveAstar(grid, startN, endN);

					}
					else {
						endN = &grid[gridPositions.y * gridWidth + gridPositions.x];
						endN->isObstacle = false;
						startOrEndPositionChange = !startOrEndPositionChange;
						SolveAstar(grid, startN, endN);

					}
				}
			}
			if (event.type == sf::Event::MouseButtonReleased) {

				if (event.mouseButton.button == sf::Mouse::Right)
				{
					rightMouseButtonHolding = false;
				}

			}
			
			if (event.type == sf::Event::MouseMoved) {
				sf::Vector2i gridPositions(event.mouseMove.x / cellSize, event.mouseMove.y / cellSize);

				if (rightMouseButtonHolding) {
					drawWall(grid, gridPositions);
					SolveAstar(grid, startN, endN);
				}
			}
		}

		window.clear();
		
		drawGrid(window, grid);

		sf::RectangleShape rectangle(sf::Vector2f(cellSize, cellSize));
		sf::Vector2i pos = (*startN).position;
		rectangle.setPosition(pos.x * cellSize, (pos.y * cellSize));
		rectangle.setFillColor(sf::Color::Green);
		window.draw(rectangle);


		if (endN != nullptr) {
			pathNode* helper = endN;
			while (helper->parent != nullptr) {
				sf::Vector2i pos = (*helper).position;
				rectangle.setPosition(pos.x * cellSize, (pos.y * cellSize));
				rectangle.setFillColor(sf::Color::Yellow);
				window.draw(rectangle);

			/*	sf::Vector2f pos1 = sf::Vector2f(helper->position*cellSize);
				sf::Vector2f pos2 = sf::Vector2f(helper->parent->position* cellSize);

				sf::Vertex line[] =
				{
					sf::Vertex(pos1),
					sf::Vertex(pos2)
				};
								window.draw(line, 2, sf::Lines);
				*/
				helper = helper->parent;

			}

		}
		pos = (*endN).position;
		rectangle.setPosition(pos.x * cellSize, (pos.y * cellSize));
		rectangle.setFillColor(sf::Color::Red);
		window.draw(rectangle);


		window.display();
	}

	return 0;
}