#include "CollisionVolume.h"
#include "Setting.h"
#include "raylibcpp.h"
#include "GameObjectDynamic.h"

CollisionVolume::CollisionVolume(CollisionVolumeConfiguration _params)
	: m_configuration(_params), 
	Visual2DDynamicNode(node::type::COLLISION_2D_SHAPE, node::renderLayer::DEBUG_NON_GUI, _params.renderState),
	m_debugDraw(true)
{
}


void CollisionVolume::Initialization()
{
	s_collisionNodes.push_back(std::dynamic_pointer_cast<GameObjectDynamic>(GetParent()->shared_from_this()));
	if (auto parentTransform = std::dynamic_pointer_cast<RendableNode>(GetParent()->shared_from_this()) )
	{
		SetWorldTransform(parentTransform->GetWorldTransform());
	}
}

void CollisionVolume::Draw()
{
	if (!m_debugDraw)
		return;
	//if (auto parent = GetParent()->shared_from_this())
	{
		//if (const auto rendableNode = std::dynamic_pointer_cast<RendableNode>(parent))
		{
			Color colour = !m_collisionPairsRaw.empty() || !m_collisionVoxels.empty() ? m_configuration.onCollisionColour : m_configuration.onNoCollisionColour;
			switch (m_configuration.type)
			{
			case CollisionVolume::VolumeType::rectangle:
				//raycpp::DrawRectangleLinesEx(rendableNode->GetWorldTransform().GetPosition(), m_configuration.dimentions, m_configuration.borderWidth, colour);
				raycpp::DrawRectangleLinesEx(GetWorldTransform().GetPosition(), m_configuration.dimentions, m_configuration.borderWidth, colour);
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

void CollisionVolume::Update()
{
	if (auto parentTransform = std::dynamic_pointer_cast<RendableNode>(GetParent()->shared_from_this()) )
	{
		SetWorldTransform(parentTransform->GetWorldTransform());
	}
}

void CollisionVolume::SetCollisionPair(std::vector<GameObjectDynamic*> _pairs)
{
	if (_pairs.empty())
		return;
	for (int i = _pairs.size() - 1; i >= 0; --i)
	{
		if (const auto obj = _pairs[i])
		{
			if (auto collisionNode = std::dynamic_pointer_cast<CollisionVolume>(obj->GetChildNode(node::type::COLLISION_2D_SHAPE).lock()))
			{
				auto id0 = collisionNode->GetUniqueID();
				auto id1 = GetUniqueID();
				if (id0 == id1)
				{
					_pairs.erase(_pairs.begin() + i);
				}
				else
				{
					if (const auto thisGameObject = GetParent()->shared_from_this())
						if(auto gameObjectDynamic = std::dynamic_pointer_cast<GameObjectDynamic>(thisGameObject))
							collisionNode->AddCollisionPair(gameObjectDynamic.get());
				}
			}
		}
	}
	m_collisionPairsRaw = _pairs;
}

void CollisionVolume::AddCollisionPair(GameObjectDynamic* obj)
{
	if (auto collisionNode = std::dynamic_pointer_cast<CollisionVolume>(obj->GetChildNode(node::type::COLLISION_2D_SHAPE).lock()))
	{
		bool exists = false;
		auto uid = GetUniqueID();
		auto otherUid = collisionNode->GetUniqueID();
		if (uid == otherUid)
			return;

		for (auto& node : m_collisionPairsRaw)
		{
			auto node0 = node->GetUniqueID();
			if (node0 == otherUid)
				exists = true;
		}

		if (!exists)
			m_collisionPairsRaw.push_back(obj);
	}
}

void CollisionVolume::SetCollisionVoxel(std::vector<QuadTreeV3*> voxels)
{
	m_collisionVoxels = voxels;
}

void CollisionVolume::ClearCollisionPairs()
{
	m_collisionPairs.clear();
	m_collisionPairsRaw.clear();
	m_collisionVoxels.clear();
}
