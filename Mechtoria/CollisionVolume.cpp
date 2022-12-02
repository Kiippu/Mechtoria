#include "CollisionVolume.h"
#include "Setting.h"
#include "raylibcpp.h"
#include "GameObjectDynamic.h"

CollisionVolume::CollisionVolume(CollisionVolumeConfiguration _params)
	: m_configuration(_params), 
	Visual2DDynamicNode(node::type::COLLISION_2D_SHAPE, 
	node::renderLayer::DEBUG_NON_GUI), 
	m_debugDraw(true)
{
}


void CollisionVolume::Initialization()
{
	s_collisionNodes.push_back(std::dynamic_pointer_cast<GameObjectDynamic>(GetParent()->shared_from_this()));
}

void CollisionVolume::Draw()
{
	if (!m_debugDraw)
		return;
	if (auto parent = GetParent()->shared_from_this())
	{
		const auto colour = m_collisionPairs.empty() ? m_configuration.onNoCollisionColour : m_configuration.onCollisionColour;
		if (const auto rendableNode = std::dynamic_pointer_cast<RendableNode>(parent))
		{
			switch (m_configuration.type)
			{
			case CollisionVolume::VolumeType::rectangle:
				raycpp::DrawRectangleLinesEx(rendableNode->GetWorldTransform().GetPosition(), m_configuration.dimentions, m_configuration.borderWidth, colour);
				break;
			/*case CollisionVolume::VolumeType::circle:
				raycpp::DrawRectangleLinesEx(rendableNode->GetWorldTransform().GetPosition(), m_configuration.dimentions, m_configuration.borderWidth, colour);
				break;*/
			default:
				break;
			}
		}
	}
}