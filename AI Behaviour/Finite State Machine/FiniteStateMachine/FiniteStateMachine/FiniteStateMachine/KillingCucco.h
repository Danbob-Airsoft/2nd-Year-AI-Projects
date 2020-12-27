#pragma once
#include "States.h"
class KillingCucco :
	public States
{
	void OnEnter();
	void OnExit();
	int Update();
};

