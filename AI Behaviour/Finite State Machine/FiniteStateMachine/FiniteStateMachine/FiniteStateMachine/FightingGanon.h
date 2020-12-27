#pragma once
#include "States.h"
class FightingGanon :
	public States
{
	void OnEnter();
	void OnExit();
	int Update();
};

