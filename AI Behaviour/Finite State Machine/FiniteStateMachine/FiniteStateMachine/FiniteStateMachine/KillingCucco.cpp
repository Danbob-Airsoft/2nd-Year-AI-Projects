#include "KillingCucco.h"
#include "iostream"

void KillingCucco::OnEnter()
{
	std::cout << "Link finds a group of Cucco. Murderous intent overtakes him..." << std::endl;
	std::cout << " What will he do now?" << std::endl;
}

void KillingCucco::OnExit()
{
}

int KillingCucco::Update()
{
	int StateValue;
	StateValue = std::rand() % 100;
	if (StateValue <= 30)
	{
		//Set State to Side Questing
		return 4;
	}
	else if (StateValue >= 31 && StateValue <= 60)
	{
		//Set State to SmashingPots
		return 2;
	}
	else
	{
		//Set State to Sailing
		return 1;
	}

	return INT_MAX;
}
