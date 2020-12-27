#include "AStarGrid.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

NodeClass::NodeClass(int x, int y)
{
	xPos = x;
	yPos = y;

	CameFrom = nullptr;
	Vistited = false;
	NodeFillColour = sf::Color::Transparent;
	NotPassable = false;
	MovementCost = 1;
}

void NodeClass::DrawNode(sf::RenderWindow& Window)
{
	NodeRect.setSize(sf::Vector2f(20.0, 20.0));
	NodeRect.setPosition(sf::Vector2f(xPos * 20, yPos * 20));
	CenterX = (xPos * NodeRect.getSize().x) + 10;
	CenterY = (yPos * NodeRect.getSize().y) + 10;
	if (NodeFillColour == sf::Color::Transparent)
	{
		NodeRect.setOutlineColor(sf::Color::White);
	}
	else
	{
		NodeRect.setOutlineColor(NodeFillColour);
	}
	NodeRect.setOutlineThickness(1);
	NodeRect.setFillColor(NodeFillColour);

	Window.draw(NodeRect);
}

void NodeClass::CheckFrontier(std::queue<NodeClass*> &Frontier, NodeClass *Target, bool &TargetFound)
{
	//------------Check Neighbours--------------
	for(int i = 0; i < Neighbours.size(); i++)
	{
		NodeClass* CurrentNeighbour = Neighbours[i];
		//Check if vistied
		if (CurrentNeighbour->Vistited == false && !CurrentNeighbour->NotPassable)
		{
			//Node has now been visited
			CurrentNeighbour->Vistited = true;
			//Check if target
			CurrentNeighbour->CameFrom = this;
			if (CurrentNeighbour != Target)
			{
				//Add to Frontier to be checked
				Frontier.push(CurrentNeighbour);
			}
			else
			{
				TargetFound = true;
				return;
			}
		}
	}
	Frontier.pop();
}

void NodeClass::FindOrigin(bool &PathGenerated, NodeClass* StartNode, std::queue<NodeClass*> &Path)
{
	//Check if Current node is target (Start Point)
	if (this == StartNode) 
	{
		PathGenerated = true;
		return;
	}
	NodeFillColour = sf::Color::Blue;
	NodeClass* Origin = this->CameFrom;
	Path.push(Origin);
	Origin->FindOrigin(PathGenerated, StartNode, Path);
}
