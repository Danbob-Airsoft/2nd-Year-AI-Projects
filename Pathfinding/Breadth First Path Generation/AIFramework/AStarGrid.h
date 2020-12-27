#pragma once
#include <SFML/Graphics.hpp>
#include <queue>
#include <iostream>

class AStarGrid
{

};

class NodeClass 
{
public:
	//Position
	int xPos;
	int yPos;

	//Actual Center
	int CenterX;
	int CenterY;

	//Has been visited
	bool Vistited;

	//Is Node not passable?
	bool NotPassable;


	//Cost to move to
	int MovementCost;

	//Node that found in frontier search
	NodeClass* CameFrom;

	//Neigbours
	std::vector<NodeClass*> Neighbours;

	//Fill Colour
	sf::Color NodeFillColour;
	sf::RectangleShape NodeRect;

	NodeClass(int x, int y);

	//Draw Node
	void DrawNode(sf::RenderWindow& Window);

	//Check Neigbour Nodes
	void CheckFrontier(std::queue<NodeClass*> &Frontier, NodeClass *Target, bool &TargetFound);

	//Find Origin of Target
	void FindOrigin(bool &PathGenerated, NodeClass* StartNode, std::queue<NodeClass*> &Path);

};

