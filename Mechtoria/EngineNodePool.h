#pragma once
#include <map>
#include <memory>
#include "Node.h"
#include "Setting.h"

class EngineNodePool : public DynamicNode
{
public:
	EngineNodePool()
		: DynamicNode(node::type::ENGINE_POOL) {}
	virtual void Initialization() override {};
	virtual void Deinitialization() override {};

	virtual void Update() override {};

	// for creatiung entities and placing them in pool automatically
	// std::weak_ptr<Node> CreateEntity(node::entity _entityType);

private:
	// for creating all entities
	// Spawner m_spwner;
};

