#include "ExploringDungeon.h"
#include "iostream"

void ExploringDungeon::OnEnter()
{
	std::cout << "Link wanders through the dungeon, on the hunt for keys!" << std::endl;
	std::cout << " What will he do now?" << std::endl;
}

void ExploringDungeon::OnExit()
{
}

int ExploringDungeon::Update()
{
	int StateValue;
	StateValue = std::rand() % 100;
	if (StateValue <= 15) 
	{
		//Set State to Sailing
		return 1;
	}
	else if (StateValue >=16 && StateValue <= 60)
	{
		//Set State to Fighting Ganon
		return 3;
	}
	else if (StateValue >= 61 && StateValue <= 80)
	{
		//Set State to SmashingPots
		return 2;
	}
	else 
	{
		//Set State to Killing Cucco
		return 5;
	}
	
	return INT_MAX;
}
