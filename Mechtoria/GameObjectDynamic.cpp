#include "GameObjectDynamic.h"
#include "FiniteStateMachine.h"

GameObjectDynamic::GameObjectDynamic(IState* state, node::renderLayer renderType, node::entity entityType)
	: Visual2DDynamicNode(node::type::GAME_OBJECT, renderType, state->GetType()), m_stateThisBelongsTo(state), m_entityType(entityType)
{}

IState* GameObjectDynamic::GetOwnerState() const
{
    return m_stateThisBelongsTo;
}

node::entity GameObjectDynamic::GetEntityType() const
{
    return m_entityType;
}
