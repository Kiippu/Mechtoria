#pragma once
#include "FiniteStateMachine.h"

class QuadTreeV2;
class GamePlayState;

class QuadTreeTestState : public IState
{
public:
	QuadTreeTestState(FiniteStateMachine* _parent, GamePlayState* _parentState);

	virtual void Initialization() override {};
	virtual void Deinitialization() override {};

	virtual void Update() override {};
	virtual void Draw() override;
private:

	GamePlayState* m_gamePlayState;
	std::unique_ptr<QuadTreeV2> m_quadTree;
	std::vector<Vec2<float>> m_quadPoints;
};


