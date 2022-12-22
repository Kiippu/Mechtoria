#include "CollisionHandler.h"
#include "SweepAndPruneCollision.h"
#include "CollisionVolume.h"
#include "QuadTreeV2.h"
#include "QuadTreeV3.h"
#include "GameObjectDynamic.h"

void CollisionHandler::Update(QuadTreeV2* quadTree)
{
	const auto nodeList = s_collisionNodes;
	for (auto& node : nodeList)
	{

	}
}

void CollisionHandler::Update(QuadTreeV3* quadTree)
{
	const auto nodeList = s_collisionNodes;
	std::vector<std::vector<std::weak_ptr<GameObjectDynamic>>> allPairs;
	for (auto& node : nodeList)
	{
		if (auto obj = node.lock())
		{
			if (const auto collisionNode = std::dynamic_pointer_cast<CollisionVolume>(obj->GetChildNode(node::type::COLLISION_2D_SHAPE).lock()))
			{
				Quad quad{ collisionNode->GetWorldTransform().GetPosition(), collisionNode->GetConfiguration().dimentions};
				std::vector<GameObjectDynamic*>objFound;
				quadTree->Query(quad, objFound);
				collisionNode->SetCollisionPair(objFound);
				std::vector<QuadTreeV3*>voxFound;
				quadTree->QueryVoxels(quad, voxFound);
				collisionNode->SetCollisionVoxel(voxFound);
				/*if (!voxFound.empty())
				{
					int i = 0;
					i++;
				}*/
			}
		}
	}
}

void CollisionHandler::Update(SweepAndPruneCollision* prune)
{
	prune->Update();
}

void CollisionHandler::ClearCollisions()
{
	const auto nodeList = s_collisionNodes;
	std::vector<std::vector<std::weak_ptr<GameObjectDynamic>>> allPairs;
	for (auto& node : nodeList)
	{
		if (auto obj = node.lock())
		{
			if (const auto collisionNode = std::dynamic_pointer_cast<CollisionVolume>(obj->GetChildNode(node::type::COLLISION_2D_SHAPE).lock()))
			{
				collisionNode->ClearCollisionPairs();
			}
		}
	}
}

void CollisionHandler::UpdateCollisionPairs(collisionList list)
{
	for (const auto& node : list)
	{
		if (auto nodeA = node[0].lock())
		{
			if (auto nodeB = node[1].lock())
			{
				if (DetectCollision(nodeA, nodeB))
				{
					/*if (const auto collisionNode = std::dynamic_pointer_cast<CollisionVolume>(nodeA->GetChildNode(node::type::COLLISION_2D_SHAPE).lock()))
						collisionNode->SetCollisionPair(nodeB);
					if (const auto collisionNode = std::dynamic_pointer_cast<CollisionVolume>(nodeB->GetChildNode(node::type::COLLISION_2D_SHAPE).lock()))
						collisionNode->SetCollisionPair(nodeA);*/
				}
			}
		}
	}
}

bool CollisionHandler::DetectCollision(std::shared_ptr<GameObjectDynamic> _nodeA, std::shared_ptr<GameObjectDynamic> _nodeB)
{
	// NodeA
	const auto topLeftA = _nodeA->GetWorldTransform().GetPosition();
	const auto collisionNodeA = std::dynamic_pointer_cast<CollisionVolume>(_nodeA->GetChildNode(node::type::COLLISION_2D_SHAPE).lock());
	const auto dimentionsA = collisionNodeA->GetConfiguration().dimentions;
	const auto bottonRightA = topLeftA + dimentionsA;
	// NodeB
	const auto topLeftB = _nodeB->GetWorldTransform().GetPosition();
	const auto collisionNodeB = std::dynamic_pointer_cast<CollisionVolume>(_nodeB->GetChildNode(node::type::COLLISION_2D_SHAPE).lock());
	const auto dimentionsB = collisionNodeB->GetConfiguration().dimentions;
	const auto bottonRightB = topLeftB + dimentionsB;

	// test collisions
	const bool isLeftInZone = (topLeftA.GetX() >= topLeftB.GetX()) && (topLeftA.GetX() <= bottonRightB.GetX());
	const bool isRightInZone = (bottonRightA.GetX() >= topLeftB.GetX()) && (bottonRightA.GetX() <= bottonRightB.GetX());
	// test nodeA top and bottom
	const bool isTopInZoneA = (topLeftA.GetY() >= topLeftB.GetY()) && (topLeftA.GetY() <= bottonRightB.GetY());
	const bool isBottomInZoneA = (bottonRightA.GetY() <= topLeftB.GetY()) && (bottonRightA.GetY() >= bottonRightB.GetY());
	// test nodeB top and bottom
	const bool isTopInZoneB = (topLeftB.GetY() >= topLeftA.GetY()) && (topLeftB.GetY() <= bottonRightA.GetY());
	const bool isBottomInZoneB = (bottonRightB.GetY() <= topLeftA.GetY()) && (bottonRightB.GetY() >= bottonRightA.GetY());
	// finalize collison results
	const bool isInTopOrBottomCollisionAB = (isTopInZoneA || isBottomInZoneA || isTopInZoneB || isBottomInZoneB);
	const bool isLeftCollisionWithYAxis = (isLeftInZone && isInTopOrBottomCollisionAB);
	const bool isRightCollisionWithYAxis = (isRightInZone && isInTopOrBottomCollisionAB);

	return (isLeftCollisionWithYAxis || isRightCollisionWithYAxis);
}