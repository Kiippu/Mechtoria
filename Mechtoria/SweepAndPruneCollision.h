#pragma once
#include "Node.h"
#include <vector>

class CollisionVolume;
class GameObjectDynamic;

class SweepAndPruneCollision
{
public:
	SweepAndPruneCollision() {};
	~SweepAndPruneCollision() {};
	void Update();

	void ClearCollisionPairs();

private:
	void UpdateCollisionPairs();
	std::vector<std::vector<std::weak_ptr<GameObjectDynamic>>> ProcessBroadPhase();
	void SortCollisionList();
	bool DetectCollision(
		std::shared_ptr<GameObjectDynamic> _nodeA, 
		std::shared_ptr<GameObjectDynamic> _nodeB
	);
};

