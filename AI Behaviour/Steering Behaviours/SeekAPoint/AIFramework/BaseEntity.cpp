#include "BaseEntity.h"
#include "UtilRandom.h"
#include <iostream>

std::vector<BaseEntity*> BaseEntity::Renderables;

BaseEntity::BaseEntity()
{
	// with no further information, we make some assumptions and set default values.
	filename = "assets\\boid.png";
	colourTint = sf::Color::White;
	Initialize();
}

BaseEntity::BaseEntity(std::string file) : filename(file)
{
	// with just a texture file, we default to a white tint (so no tint at all).
	colourTint = sf::Color::White;
	Initialize();
}

BaseEntity::BaseEntity(std::string file, sf::Color colour) : filename(file), colourTint(colour)
{
	Initialize();
}

BaseEntity::~BaseEntity()
{
	
}

void BaseEntity::Think()
{
	// build a new position vector by adding a scaled version of the velocity vector
	sf::Vector2f pos = getPosition() + (velocity * 0.1f);
	// update our position
	setPosition(pos);
	if ((pos.x < Target.x + 10 && pos.x > Target.x - 10) && (pos.y > Target.y - 10 && pos.y < Target.y + 10)) 
	{
		FindNewPosition();
	}
}

void BaseEntity::FindNewPosition()
{
	//Stop entity
	velocity = sf::Vector2f(0, 0);
	//Get position within screen
	float Randomx = UtilRandom::instance()->GetRange(0, 800);
	std::cout << Randomx << std::endl;
	float Randomy = UtilRandom::instance()->GetRange(0, 600);
	std::cout << Randomy<< std::endl;
	Target = sf::Vector2f(Randomx, Randomy);

	sf::Vector2f Currentpos = this->getPosition();

	//Get Rotation to point
	float tempx = Target.x - Currentpos.x;
	float tempy = Target.y - Currentpos.y;
	float angle = atan2(tempy, tempx);
	std::cout << angle;

	// set our rotation value
	setRotation(angle * 180 /M_PI);

	// and assign a velocity, we need to convert angle to radians so it plays nicely with cos and sin.
	velocity = sf::Vector2f(cos(angle), sin(angle));
}

void BaseEntity::Initialize()
{
	// set a default position - could be randomised.
	setPosition(350.0f, 350.0f);

	// load the texture into memory
	texture.loadFromFile(filename);
	texture.setSmooth(true);
	// and provide the sprite with a pointer to the texture object
	// if the texture object is destroyed (goes out of scope etc) then the sprite will display weirdly
	sprite.setTexture(texture);
	// set up our colour tint
	sprite.setColor(colourTint);
	
	// select a random angle
	//float angle = UtilRandom::instance()->GetRange(0, 361);

	//Get Random screen point
	FindNewPosition();

	// add the entity to the list of renderables.
	Renderables.push_back(this);
}