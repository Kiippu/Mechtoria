#pragma once
#include "FiniteStateMachine.h"
#include <array>
#include <vector>
#include <map>
#include "GameObjectDynamic.h"
#include "SweepAndPruneCollision.h"

class GameObjectController;
extern std::map<IState::StateType,std::map<node::renderLayer, std::vector<RendableNode*>>> s_rendableNodes;

class GamePlayState :
    public IState
{
public:
	GamePlayState(FiniteStateMachine* _parent);
	virtual ~GamePlayState() {};

	virtual void Initialization() override;
	virtual void Deinitialization() override {};

	virtual void Update() override;
	virtual void Draw() override;

	void AddGameActor(std::shared_ptr<GameObjectDynamic> _actor, node::entity _type, IState::StateType, node::renderLayer);

	std::array<std::shared_ptr<GameObjectController>,4>& GetGameObjectControllers();
	
private:
	std::array<std::shared_ptr<GameObjectController>, 4> m_controllers;
	std::map<node::entity,std::vector<std::shared_ptr<GameObjectDynamic>>> m_gameActors;
	SweepAndPruneCollision m_collision;
};

