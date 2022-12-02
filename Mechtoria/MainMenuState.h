#pragma once
#include "FiniteStateMachine.h"
class MainMenuState : public IState
{
public:
	MainMenuState(FiniteStateMachine* _parent);
	virtual ~MainMenuState() {};

	virtual void Initialization() override {};
	virtual void Deinitialization() override {};

	virtual void Update() override {};

	virtual void Draw() override {};
};

