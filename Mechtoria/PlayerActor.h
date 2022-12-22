#pragma once
#include "GameObjectDynamic.h"

class PlayerActor : public GameObjectDynamic
{
public:
	PlayerActor(IState* state, node::renderLayer renderType, node::entity entityType);

	//virtual void Update() override;

	virtual void Initialization() override;

private:

};

