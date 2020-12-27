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

void NodeClass::CheckFrontier(std::priority_queue<Node_P, std::vector<Node_P>, std::greater<Node_P>> &Frontier,
	NodeClass *Target, bool &TargetFound)
{
	if (this == Target) 
	{
		TargetFound = true;
		return;
	}

	//------------Check Neighbours--------------
	for(int i = 0; i < Neighbours.size(); i++)
	{
		NodeClass* CurrentNeighbour = Neighbours[i];
		if (!CurrentNeighbour->NotPassable) 
		{
			int newDist = this->TentativeDistance + CurrentNeighbour->MovementCost;

			if (newDist < CurrentNeighbour->TentativeDistance)
			{
				CurrentNeighbour->TentativeDistance = this->TentativeDistance + CurrentNeighbour->MovementCost;

				double Priority = std::abs(Target->xPos - CurrentNeighbour->xPos) + std::abs(Target->yPos - CurrentNeighbour->yPos);

				CurrentNeighbour->TentativeDistance = CurrentNeighbour->TentativeDistance + Priority;

				CurrentNeighbour->CameFrom = this;

				if (CurrentNeighbour != Target)
				{
					//Add to Frontier to be checked
					Frontier.push(std::make_pair(CurrentNeighbour->TentativeDistance, CurrentNeighbour));
				}
				else
				{
					TargetFound = true;
					return;
				}
			}
		}
	}
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
