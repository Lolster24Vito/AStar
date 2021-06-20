#pragma once
#include <SFML/Graphics.hpp>
#include<vector>
struct pathNode
{
	bool isObstacle = false;
	bool isVisited = false;
	float gCost; //distance from starting node local
	float hCost; //distance from end node---- global goal
	float fCost; //h+g
	sf::Vector2i position;
	std::vector<pathNode*> neighbours;
	pathNode* parent;

	void calculateFCost() {
		fCost = gCost + hCost;
	}
	
};

