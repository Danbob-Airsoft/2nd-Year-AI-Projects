// FiniteStateMachine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "ExploringDungeon.h"
#include "KillingCucco.h"
#include "Sailing.h"
#include "SideQuests.h"
#include "SmashingPots.h"
#include "FightingGanon.h"
#include <string>


ExploringDungeon* MyDungeon = new ExploringDungeon();
KillingCucco* MyCucco = new KillingCucco();
Sailing* MySailing = new Sailing();
SideQuests* MyQuest = new SideQuests();
SmashingPots* MyPots = new SmashingPots();
FightingGanon* MyGanon = new FightingGanon();

States* CurrentState = MySailing;

bool Running = true;

int main()
{
	while (Running == true) 
	{
		int NewState = CurrentState->Update();
		if (NewState == 1) 
		{
			CurrentState = MySailing;
		}
		else if (NewState == 2)
		{
			CurrentState = MyPots;
		}
		else if (NewState == 3) 
		{
			CurrentState = MyGanon;
		}
		else if (NewState == 4)
		{
			CurrentState = MyQuest;
		}
		else if (NewState == 5)
		{
			CurrentState = MyCucco;
		}
		else if (NewState == 6)
		{
			CurrentState = MyDungeon;
		}
		else 
		{
			//Game End
			std::string temp;
			std::getline(std::cin, temp);
			Running = false;
		}
		CurrentState->OnEnter();
		std::string temp;
		std::getline(std::cin, temp);
	}
}
