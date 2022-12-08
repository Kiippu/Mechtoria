#pragma once
#include "FiniteStateMachine.h"

class GamePlayState;
class QuadTreeV3;
class CollisionHandler;

class SideScrollState	: public IState
{
public:
	SideScrollState(FiniteStateMachine* _parent, GamePlayState* _parentState);

	virtual void Initialization() override;

	virtual void Update() override;
	virtual void Draw() override;

private:

	GamePlayState* m_gamePlayState;
	std::unique_ptr<QuadTreeV3> m_quadTree;
	std::unique_ptr<CollisionHandler> m_collision;
};

