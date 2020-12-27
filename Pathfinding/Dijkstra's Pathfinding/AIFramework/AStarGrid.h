#pragma once
#include <SFML/Graphics.hpp>
#include <queue>
#include <iostream>
#include <unordered_map>

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
	int TentativeDistance;

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

	typedef std::pair<int, NodeClass*> Node_P;

	//Check Neigbour Nodes
	void CheckFrontier(std::priority_queue<Node_P, std::vector<Node_P>, std::greater<Node_P>>& Frontier,
		NodeClass *Target, bool &TargetFound);

	//Find Origin of Target
	void FindOrigin(bool &PathGenerated, NodeClass* StartNode, std::queue<NodeClass*> &Path);

};

