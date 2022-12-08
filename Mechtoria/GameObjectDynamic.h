#pragma once
#include "Node.h"
#include "Setting.h"

class IState;

class GameObjectDynamic : public Visual2DDynamicNode
{
public:
	GameObjectDynamic(IState* state, node::renderLayer renderType, node::entity entityType);

	IState* GetOwnerState() const;
	node::entity GetEntityType() const;

private:
	IState* m_stateThisBelongsTo;
	node::entity m_entityType;
};

