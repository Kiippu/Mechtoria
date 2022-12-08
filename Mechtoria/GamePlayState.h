#pragma once
#include "FiniteStateMachine.h"
#include <array>
#include <vector>
#include <map>
#include "GameObjectDynamic.h"
#include "SweepAndPruneCollision.h"
#include "QuadTreeV2.h"
#include "QuadTreeV3.h"
#include "CollisionHandler.h"

class GameObjectController;
extern std::map<state::type,std::map<node::renderLayer, std::vector<RendableNode*>>> s_rendableNodes;

class GamePlayState :
    public IState
{
public:
	GamePlayState(FiniteStateMachine* _parent);
	virtual ~GamePlayState() {};

	virtual void Initialization() override;

	virtual void Update() override;
	virtual void Draw() override;

	void AddGameActor(std::shared_ptr<GameObjectDynamic> _actor);

	std::array<std::shared_ptr<GameObjectController>,4>& GetGameObjectControllers();
	
private:
	std::array<std::shared_ptr<GameObjectController>, 4> m_controllers;
	std::map<node::entity,std::vector<std::shared_ptr<GameObjectDynamic>>> m_gameActors;
	SweepAndPruneCollision m_sweepAndPrune;
	std::map<state::type,std::unique_ptr<QuadTreeV2>> m_world;
	std::map<state::type,std::unique_ptr<QuadTreeV3>> m_worldV2;
	CollisionHandler m_collision;
	bool m_initialisedActors = false;

private:
	void InitActors(std::vector<std::shared_ptr<GameObjectDynamic>>& node);
};

