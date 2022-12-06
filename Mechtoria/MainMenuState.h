#pragma once
#include "FiniteStateMachine.h"
class MainMenuState : public IState
{
public:
	MainMenuState(FiniteStateMachine* _parent);
	virtual ~MainMenuState() {};

	virtual void Update() override {};

	virtual void Draw() override {};
};

