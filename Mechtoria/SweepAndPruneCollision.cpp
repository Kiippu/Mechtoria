#include "SweepAndPruneCollision.h"
#include <algorithm>
#include "CollisionVolume.h"
#include "GameObjectDynamic.h"

void SweepAndPruneCollision::Update()
{
	// reset collision pair data and remove invalid pointers
	ClearCollisionPairs();
	UpdateCollisionPairs();
}

void SweepAndPruneCollision::ClearCollisionPairs()
{
	for (auto node = s_collisionNodes.begin(); node != s_collisionNodes.end() ; ++node)
	{
		if (auto lockedNode = node->lock())
		{
			if (const auto collisionNode = std::dynamic_pointer_cast<CollisionVolume>(lockedNode->GetChildNode(node::type::COLLISION_2D_SHAPE).lock()))
				collisionNode->ClearCollisionPairs();
		}
		else
		{
			s_collisionNodes.erase(node);
		}
	}
}

void SweepAndPruneCollision::UpdateCollisionPairs()
{
	const auto nodesPairs = ProcessBroadPhase();
	for (const auto& node : nodesPairs)
	{
		if (auto nodeA = node[0].lock())
		{
			if (auto nodeB = node[1].lock())
			{
				if (DetectCollision(nodeA, nodeB))
				{
					if(const auto collisionNode = std::dynamic_pointer_cast<CollisionVolume>(nodeA->GetChildNode(node::type::COLLISION_2D_SHAPE).lock()))
						collisionNode->SetCollisionPair(nodeB);
					if (const auto collisionNode = std::dynamic_pointer_cast<CollisionVolume>(nodeB->GetChildNode(node::type::COLLISION_2D_SHAPE).lock()))
						collisionNode->SetCollisionPair(nodeA);
				}
			}
		}
	}
}

std::vector<std::vector<std::weak_ptr<GameObjectDynamic>>> SweepAndPruneCollision::ProcessBroadPhase()
{
	SortCollisionList();

	auto& nodeList = s_collisionNodes;

	std::vector<std::vector<std::weak_ptr<GameObjectDynamic>>> allPairs;
	std::vector<std::weak_ptr<GameObjectDynamic>> activeList; //temporary list

	for (auto i = 0; i < nodeList.size(); ++i) {
		for (auto j = 0; j < activeList.size(); ++j) {
			//cout << "comparing " << nodeList[i]->name << " and " << activeList[j]->name << endl;
			if (const auto nodeA = nodeList[i].lock())
			{
				if (const auto nodeB = nodeList[j].lock())
				{
					auto maxB = nodeB->GetWorldTransform().GetPosition().GetX();
					if (const auto collisionNodeB = std::dynamic_pointer_cast<CollisionVolume>(nodeB->GetChildNode(node::type::COLLISION_2D_SHAPE).lock()))
						maxB += collisionNodeB->GetConfirgutrtion().dimentions.GetX();
					const auto minA = nodeA->GetWorldTransform().GetPosition().GetX();
					if (minA > maxB) {
						//cout << " object is poped " << activeList[j]->name << endl;
						activeList.erase(activeList.begin() + j);
						j--;
					}
					else {
						//std::cout << " reported pair " << nodeList[i]->name << " and " << activeList[j]->name << std::endl;
						std::vector<std::weak_ptr<GameObjectDynamic>> mpair;
						mpair.push_back(nodeList[i]);
						mpair.push_back(activeList[j]);
						allPairs.push_back(mpair);
					}
				}
			}
		}
		//cout << nodeList[i]->name << " is added to activeList  " << endl;
		activeList.push_back(nodeList[i]);
	}
	return allPairs;
}

void SweepAndPruneCollision::SortCollisionList()
{
	// sort from screen position left to right
	auto& v = s_collisionNodes;
	sort(v.begin(), v.end(), [=](std::weak_ptr<GameObjectDynamic> a, std::weak_ptr<GameObjectDynamic> b) -> bool {
		const auto nodeA = a.lock();
		const auto nodeB = b.lock();
		if (!nodeA && !nodeB)
			return false;

		const auto minA = nodeA->GetWorldTransform().GetPosition().GetX();
		const auto minB = nodeB->GetWorldTransform().GetPosition().GetX();

		return (minA < minB);
	});
}

bool SweepAndPruneCollision::DetectCollision(std::shared_ptr<GameObjectDynamic> _nodeA, std::shared_ptr<GameObjectDynamic> _nodeB)
{
	// NodeA
	const auto topLeftA = _nodeA->GetWorldTransform().GetPosition();
	const auto collisionNodeA = std::dynamic_pointer_cast<CollisionVolume>(_nodeA->GetChildNode(node::type::COLLISION_2D_SHAPE).lock());
	const auto dimentionsA = collisionNodeA->GetConfirgutrtion().dimentions;
	const auto bottonRightA = topLeftA + dimentionsA;
	// NodeB
	const auto topLeftB = _nodeB->GetWorldTransform().GetPosition();
	const auto collisionNodeB = std::dynamic_pointer_cast<CollisionVolume>(_nodeB->GetChildNode(node::type::COLLISION_2D_SHAPE).lock());
	const auto dimentionsB = collisionNodeB->GetConfirgutrtion().dimentions;
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
