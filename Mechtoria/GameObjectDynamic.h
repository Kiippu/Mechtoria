#pragma once
#include "Node.h"
#include "Setting.h"
class GameObjectDynamic : public Visual2DDynamicNode
{
public:
	GameObjectDynamic()
		: Visual2DDynamicNode(node::type::GAME_OBJECT, node::renderLayer::SKIP_NODE)
	{}

	virtual void Initialization() override {};
	virtual void Deinitialization() override {};

};

