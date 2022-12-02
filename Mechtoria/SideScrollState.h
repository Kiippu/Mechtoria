#pragma once
#include "FiniteStateMachine.h"

class GamePlayState;

class SideScrollState	: public IState
{
public:
	SideScrollState(FiniteStateMachine* _parent, GamePlayState* _parentState);

	virtual void Initialization() override {};
	virtual void Deinitialization() override {};

	virtual void Update() override {};

	virtual void Draw() override {};
private:

	GamePlayState* m_gamePlayState;
};
