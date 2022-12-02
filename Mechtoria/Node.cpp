#include "Node.h"

//std::map<node::renderLayer, std::vector<RendableNode*>> s_rendableNodes = std::map<node::renderLayer, std::vector<RendableNode*>>();
std::vector<std::weak_ptr<GameObjectDynamic>> s_collisionNodes;