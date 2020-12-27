#include "SmashingPots.h"
#include "iostream"

void SmashingPots::OnEnter()
{
	std::cout << "Link spots some pots in the corner of the room. Heroe's gotta do what a Heroe's gotta do!" << std::endl;
	std::cout << " What will Link do now?" << std::endl;
}

void SmashingPots::OnExit()
{
}

int SmashingPots::Update()
{
	int StateValue;
	StateValue = std::rand() % 100;
	if (StateValue <= 30)
	{
		//Set State to Sailing
		return 1;
	}
	else if (StateValue >= 31 && StateValue <= 60)
	{
		//Set State to Side Questing
		return 4;
	}
	else if (StateValue >= 61 && StateValue <= 80)
	{
		//Set State to Killing Cucco
		return 5;
	}
	else
	{
		//Set State to Entering Dungeon
		return 6;
	}

	return INT_MAX;
}
