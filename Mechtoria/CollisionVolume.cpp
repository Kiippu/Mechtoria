#include "CollisionVolume.h"
#include "Setting.h"
#include "QuadTreeV3.h"
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
		const auto halfDimsY = Vec2<float>(m_configuration.dimentions.GetX(), m_configuration.dimentions.GetY() * 0.5f);
		const auto halfDimsX = Vec2<float>(m_configuration.dimentions.GetX() * 0.5f, m_configuration.dimentions.GetY());

		const float quartDimX = m_configuration.dimentions.GetX() * 0.20f;
		const float quartDimY = m_configuration.dimentions.GetY() * 0.25f;
		const float horizontalTopLeft = (m_configuration.dimentions.GetY() * 0.75f) /2.f;

		const Vec2<float> verticalDims = { m_configuration.dimentions.GetX()*0.6f, halfDimsY.GetY()};
		const Vec2<float> horizontalDims = { halfDimsX.GetX(), quartDimY};

		SetWorldTransform(parentTransform->GetWorldTransform());

		// figure this out... 
		// i do not have the sub collision correct.. need to white board them.


		m_blockingCollision.insert(
			std::make_pair(
				physics::directionBlock::NORTH, 
				Quad{ {quartDimX, 0.f}, verticalDims
				}));
		m_blockingCollision.insert(
			std::make_pair(
				physics::directionBlock::EAST,
				Quad{ { halfDimsX.GetX(), horizontalTopLeft}, horizontalDims
				}));
		m_blockingCollision.insert(
			std::make_pair(
				physics::directionBlock::SOUTH,
				Quad{ { quartDimX, halfDimsY.GetY() }, verticalDims
				}));
		m_blockingCollision.insert(
			std::make_pair(
				physics::directionBlock::WEST,
				Quad{ {0.f, horizontalTopLeft}, horizontalDims
				}));
	}
}

void CollisionVolume::Draw()
{
	if (!m_debugDraw)
		return;

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

	int flag = 0x0;
	int trackFlags = 0x0;
	for (auto& voxel : m_collisionVoxels)
	{
		flag = TestBlockingCollision(voxel->GetBounds());
		if (flag == (int)physics::directionBlock::NO_DIRECTION)
			continue;
		DrawBlockingCollision(physics::directionBlock::NORTH, flag);
		DrawBlockingCollision(physics::directionBlock::EAST, flag);
		DrawBlockingCollision(physics::directionBlock::SOUTH, flag);
		DrawBlockingCollision(physics::directionBlock::WEST, flag);
		// exit if all flags exist
		if ((trackFlags |= flag) = 0xf)
			break;
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
	for (int i = (int)_pairs.size() - 1; i >= 0; --i)
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
			auto nodeID = node->GetUniqueID();
			if (nodeID == otherUid)
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

const Quad* CollisionVolume::GetBlockingCollisionQuad(physics::directionBlock side)
{
	auto itr = m_blockingCollision.find(side);
	if (itr != m_blockingCollision.cend())
		return &itr->second;
	return nullptr;
}

const int CollisionVolume::TestBlockingCollision(const Quad& quad)
{
	int flag = 0x0;
	for (auto& block : m_blockingCollision)
	{
		Quad temp{GetWorldTransform().GetPosition() + block.second.topLeft, block.second.dimentions};
		if (temp.Intersects(quad))
			flag |= (int)block.first;
	}
	return flag;
}

void CollisionVolume::ClearCollisionPairs()
{
	m_collisionPairs.clear();
	m_collisionPairsRaw.clear();
	m_collisionVoxels.clear();
}

void CollisionVolume::DrawBlockingCollision(physics::directionBlock side, const int& flag)
{
	if (flag & (int)side)
	{
		auto itr = m_blockingCollision.find(side);
		if (itr == m_blockingCollision.cend())
			return;
		Vec2<float> pos =  GetWorldTransform().GetPosition() + itr->second.topLeft;
		raycpp::DrawRectangleLinesEx(pos, itr->second.dimentions, 3, BLUE);
	}
}
