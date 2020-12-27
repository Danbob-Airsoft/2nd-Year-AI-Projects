#include "BaseEntity.h"
#include "UtilRandom.h"

std::vector<BaseEntity*> BaseEntity::Renderables;

BaseEntity::BaseEntity(NodeClass* ActiveGrid[40][40])
{
	// with no further information, we make some assumptions and set default values.
	filename = "assets\\boid.png";
	colourTint = sf::Color::White;
	Initialize(ActiveGrid);
}

BaseEntity::BaseEntity(std::string file, NodeClass* ActiveGrid[40][40]) : filename(file)
{
	// with just a texture file, we default to a white tint (so no tint at all).
	colourTint = sf::Color::White;
	Initialize(ActiveGrid);
}

BaseEntity::BaseEntity(std::string file, sf::Color colour, NodeClass* ActiveGrid[40][40]) : filename(file), colourTint(colour)
{
	Initialize(ActiveGrid);
}

BaseEntity::~BaseEntity()
{
	
}

void BaseEntity::Think(NodeClass* ActiveGrid[40][40])
{
	FindCurrentTarget(ActiveGrid);
}

void BaseEntity::FindCurrentTarget(NodeClass* ActiveGrid[40][40])
{
	NodeClass* CurrentTarget = Path.front();
	//Rotate Towards Target
	float Angle = -atan2(CurrentTarget->CenterX - getPosition().x, CurrentTarget->CenterY - getPosition().y)
		* 180 / M_PI;
	Angle += 90;

	//Rotate Agent
	setRotation(Angle);

	//Work out forward Velocity
	CalculateForwardVelocity();

	//Move Towards Target
	// build a new position vector by adding a scaled version of the velocity vector
	sf::Vector2f pos = getPosition() + (velocity * 1.0f);
	// update our position
	setPosition(pos);

	//Check if arrived at target
	ArrivalCheck(CurrentTarget, ActiveGrid);
}

void BaseEntity::ArrivalCheck(NodeClass* CurrentTarget, NodeClass* ActiveGrid[40][40])
{
	//Check if within distance to target
	if ((CurrentTarget->CenterX - getPosition().x < 2 && CurrentTarget->CenterX - getPosition().x > -2)
		&& (CurrentTarget->CenterY - getPosition().y < 2 && CurrentTarget->CenterY - getPosition().y > -2))
	{
		if (CurrentTarget->MovementCost != 1)
		{
			CurrentTarget->NodeFillColour = sf::Color::Yellow;
		}
		else 
		{
			CurrentTarget->NodeFillColour = sf::Color::Transparent;
		}
		//if True
		//Check if Path size is one
		if (Path.size() == 1) 
		{
			//Set Target Node to Start Node
			StartNode = CurrentTarget;
			//Get New Target Node
			Target = GetRandomNode(ActiveGrid);
			//Generate Path
			GeneratePath(ActiveGrid);
			Target->NodeFillColour = sf::Color::Magenta;
		}
		//Pop Node from front of Path
		Path.pop();
	}
	//Else Continue towards current target
}

NodeClass* BaseEntity::GetRandomNode(NodeClass* ActiveGrid[40][40])
{
	bool NodeAcceptable = false;
	NodeClass* GenTarget = ActiveGrid[0][0];
	while (!NodeAcceptable) 
	{
		int Rx = UtilRandom::instance()->GetRange(1, 39);
		int Ry = UtilRandom::instance()->GetRange(1, 39);

		GenTarget = ActiveGrid[Rx][Ry];
		//If Node is not impassable then it is an acceptable target
		if (GenTarget->NotPassable == false)
		{
			int ImpassableNeighbours = 0;
			for (int i = 0; i < GenTarget->Neighbours.size(); i++) 
			{
				if (GenTarget->Neighbours[i]->NotPassable) 
				{
					ImpassableNeighbours++;
				}
			}
			if (ImpassableNeighbours != GenTarget->Neighbours.size()) 
			{
				std::cout << GenTarget->xPos<< std::endl;
				std::cout << GenTarget->yPos << std::endl;
				NodeAcceptable = true;
			}
		}
	}
	return GenTarget;
}

void BaseEntity::CalculateForwardVelocity()
{
	// Get Current Angle
	float angle = getRotation();
	// and assign a velocity, we need to convert angle to radians so it plays nicely with cos and sin.
	velocity = sf::Vector2f(cos(angle * M_PI / 180), sin(angle * M_PI / 180));
}

void BaseEntity::GeneratePath(NodeClass* ActiveGrid[40][40])
{
	//------------------------------ Dijkstra's From Start to Goal -----------------------------------------
	//Make Frontier
	std::queue<NodeClass*> Frontier;
	bool TargetFound = false;

	//Add Start Node to Frontier
	Frontier.push(StartNode);
	//While Frontier is not empty
	while (Frontier.size() > 0 && TargetFound == false)
	{
		//For each node in Frontier
		NodeClass* CurrentNode = Frontier.front();
		CurrentNode->CheckFrontier(Frontier, Target, TargetFound);
	}
	if(Frontier.size() == 0)
		std::cout << "Target Not Found" << std::endl;
	else
		std::cout << "Target Found" << std::endl;







	//-------------------------------- Use Targets Came from to work back to start -------------------------
	bool PathGenerated = false;
	Path.push(Target);

	while (PathGenerated == false)
	{
		NodeClass* PathNode = Path.back();
		PathNode->FindOrigin(PathGenerated, StartNode, Path);
	}
	std::cout << "Path Generated" << std::endl;

	//------------------------------------------ Reset Grid --------------------------------------------------------
	for each (NodeClass * Node in ActiveGrid) 
	{
		Node->CameFrom = nullptr;
		Node->Vistited = false;
		if (Node->MovementCost != 1) 
		{
			Node->NodeFillColour = sf::Color::Yellow;
		}
	}

	//-------------------------------------- Flip Path to put Target Node at the back ------------------------------
	std::stack<NodeClass*> Stack;
	while (!Path.empty()) 
	{
		Stack.push(Path.front());
		Path.pop();
	}
	bool StartAdded = false;
	while (!Stack.empty()) 
	{
		//Check if start node is already in path to prevent being added at the end
		if (Stack.top() != StartNode) 
		{
			Path.push(Stack.top());
		}
		else if (Stack.top() == StartNode && StartAdded == false) 
		{
			StartAdded = true;
			Path.push(Stack.top());
		}
		Stack.pop();
	}
	std::cout << "Path Flipped" << std::endl;
}

void BaseEntity::Initialize(NodeClass* ActiveGrid[40][40])
{
	std::cout << "Start is: " << std::endl;
	StartNode = GetRandomNode(ActiveGrid);
	std::cout << "Target is: " << std::endl;
	Target = GetRandomNode(ActiveGrid);
	
	// load the texture into memory
	texture.loadFromFile(filename);
	texture.setSmooth(true);
	// and provide the sprite with a pointer to the texture object
	// if the texture object is destroyed (goes out of scope etc) then the sprite will display weirdly
	sprite.setTexture(texture);

	sprite.setOrigin(sprite.getTexture()->getSize().x / 2, sprite.getTexture()->getSize().y / 2);

	// set a default position - could be randomised.
	setPosition(StartNode->CenterX, StartNode->CenterY);

	// set up our colour tint
	sprite.setColor(colourTint);
	sprite.setRotation(90);
	sprite.setScale(0.2f, 0.2f);

	// add the entity to the list of renderables.
	Renderables.push_back(this);

	GeneratePath(ActiveGrid);
}