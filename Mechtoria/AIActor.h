#pragma once
#include "GameObjectDynamic.h"
class AIActor : public GameObjectDynamic
{
public:
	AIActor(IState* state, node::renderLayer renderType, node::entity entityType);

	//virtual void Update() override;

	virtual void Initialization() override;
};

