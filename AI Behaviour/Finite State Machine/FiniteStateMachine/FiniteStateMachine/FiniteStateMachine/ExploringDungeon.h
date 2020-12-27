#pragma once
#include "States.h"
class ExploringDungeon :
	public States
{
	void OnEnter();
	void OnExit();
	int Update();
};

