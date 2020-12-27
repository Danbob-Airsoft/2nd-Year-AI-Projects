#include "SideQuests.h"
#include "iostream"

void SideQuests::OnEnter()
{
	std::cout << "Link finds a man who has lost his shoe in the Great Sea. Time to go fishing!" << std::endl;
	std::cout << " What will he do now?" << std::endl;
}

void SideQuests::OnExit()
{
}

int SideQuests::Update()
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
