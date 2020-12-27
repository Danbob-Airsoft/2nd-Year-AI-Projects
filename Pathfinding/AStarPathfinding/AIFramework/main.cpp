#include <iostream>
#include <SFML/Graphics.hpp>
#include <random>
#include <queue>
#include <deque>

#include "BaseEntity.h"
#include "UtilRandom.h"
#include "AStarGrid.h"

int main()
{
	// construct a window that uses a resolution of 800 x 600
	sf::RenderWindow window(sf::VideoMode(800, 800), "AI Framework");

	// because we're running as a console application, we still get access to the console, so can use std::cout to print to it
	std::cout << "Constructed SFML Window" << std::endl;

	const int Collums = 40;
	const int Rows = 40;

	//---------Generate Node Array------------
	NodeClass* Grid[40][40];
	for (int i = 0; i < Collums; i++) 
	{
		for (int j = 0; j < Rows; j++) 
		{
			Grid[i][j] = new NodeClass(i, j);
			Grid[i][j]->DrawNode(window);
		}
	}

	//-----------------Build Neighbors---------------------
	for each (NodeClass * Node in Grid)
	{
		Node->Vistited = false;
		if (Node->yPos != 0)
		{
			Node->Neighbours.push_back(Grid[Node->xPos][Node->yPos - 1]);
		}
		if (Node->yPos != 39)
		{
			Node->Neighbours.push_back(Grid[Node->xPos][Node->yPos + 1]);
		}
		if (Node->xPos != 0)
		{
			Node->Neighbours.push_back(Grid[Node->xPos - 1][Node->yPos]);
		}
		if (Node->xPos != 39)
		{
			Node->Neighbours.push_back(Grid[Node->xPos + 1][Node->yPos]);
		}
	}

	//Generate Obstacles
	for (int i = 0; i < 400; i++) 
	{
		int RandomX = UtilRandom::instance()->GetRange(0, 39);
		int RandomY = UtilRandom::instance()->GetRange(0, 39);
		NodeClass* RandomNode = Grid[RandomX][RandomY];
		RandomNode->NotPassable = true;
		RandomNode->NodeFillColour = sf::Color::Red;
	}
	std::cout << "Generated Obstacles" << std::endl;

	//Generate Random more costly tiles
	for (int i = 0; i < 100; i++) 
	{
		int RandomX = UtilRandom::instance()->GetRange(0, 39);
		int RandomY = UtilRandom::instance()->GetRange(0, 39);
		NodeClass* RandomNode = Grid[RandomX][RandomY];
		if (!RandomNode->NotPassable) 
		{
			RandomNode->NodeFillColour = sf::Color::Yellow;
			RandomNode->MovementCost = 10;
		}
	}
	std::cout << "More Costly tile generated" << std::endl;

	// construct 1 base entity
	for (int i = 0; i < 1; i++) 
	{
		BaseEntity *boid = new BaseEntity(Grid);

		boid->Target->NodeFillColour = sf::Color::Magenta;
	}
	std::cout << "Agent Created" << std::endl;

	// our game loop
	while (window.isOpen())
	{
		// clear the window before drawing anything to it
		window.clear();

		// poll for events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) 
			{
				window.close();
			}
		}

		//Redraw Grid
		for each (NodeClass *Node in Grid)
		{
			Node->DrawNode(window);
		}

		// update all our entities
		for ( auto entity : BaseEntity::Renderables )
		{
			entity->Think(Grid);
		}

		// and then draw them
		for (auto entity : BaseEntity::Renderables)
		{
			window.draw(entity->GetSprite(), entity->getTransform());
		}

		// swap the frame buffers
		window.display();
	}

	return 0;
}
