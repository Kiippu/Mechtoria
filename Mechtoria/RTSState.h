#pragma once
#include "FiniteStateMachine.h"
#include "GamePlayState.h"

class RTSState : public IState
{
public:
    RTSState(FiniteStateMachine* _parent, GamePlayState* _parentState);

	virtual void Initialization() override;

	virtual void Update() override;
	virtual void Draw() override;

private:

	GamePlayState* m_gamePlayState;
	std::unique_ptr<QuadTreeV3> m_quadTree;
	std::unique_ptr<CollisionHandler> m_collision;
};

