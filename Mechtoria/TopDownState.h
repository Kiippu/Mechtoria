#pragma once
#include "FiniteStateMachine.h"

class GamePlayState;
struct Voxel;

class TopDownState : public IState
{
public:
	TopDownState(FiniteStateMachine* _parent, GamePlayState* _parentState);
	virtual ~TopDownState() {};

	virtual void Initialization() override;

private:

	GamePlayState* m_gamePlayState;

};