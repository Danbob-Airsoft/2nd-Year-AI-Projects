#include "Sailing.h"
#include "iostream"

void Sailing::OnEnter()
{
	std::cout << "Link Sets sail on the open water. Looking for adventure!" << std::endl;
	std::cout << " What will he do now?" << std::endl;
}

void Sailing::OnExit()
{
}

int Sailing::Update()
{
	int StateValue;
	StateValue = std::rand() % 100;
	if (StateValue <= 30)
	{
		//Set State to Smashing Pots
		return 2;
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
