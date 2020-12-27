#pragma once
#include "States.h"
class Sailing :
	public States
{
	void OnEnter();
	void OnExit();
	int Update();
};

