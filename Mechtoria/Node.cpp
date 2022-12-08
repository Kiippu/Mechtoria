#include "Node.h"
#include "GamePlayState.h"

//std::map<node::renderLayer, std::vector<RendableNode*>> s_rendableNodes = std::map<node::renderLayer, std::vector<RendableNode*>>();
std::vector<std::weak_ptr<GameObjectDynamic>> s_collisionNodes;

RendableNode::RendableNode(node::renderLayer _renderLayer, state::type stateType)
	: m_renderLayer(_renderLayer)
{
	if(stateType != state::type::SKIP)
		s_rendableNodes[stateType][_renderLayer].push_back(this);
}
