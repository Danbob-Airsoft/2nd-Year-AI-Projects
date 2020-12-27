#pragma once
#include "States.h"
class SideQuests :
	public States
{
	void OnEnter();
	void OnExit();
	int Update();
};

