#include "FightingGanon.h"
#include "iostream"

void FightingGanon::OnEnter()
{
	std::cout << "Link reaches the end of the dungeon... The showdown begins!" << std::endl;
	std::cout << " After a long battle he stands victorious! His adventure is over!" << std::endl;
}

void FightingGanon::OnExit()
{
}

int FightingGanon::Update()
{
	//End Loop
	return 9;
}
