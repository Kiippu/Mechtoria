#pragma once
#include <memory>
#include <vector>

class SweepAndPruneCollision;
class GameObjectDynamic;
class QuadTreeV2;
class QuadTreeV3;

using collisionList = std::vector<std::vector<std::weak_ptr<GameObjectDynamic>>>;

class CollisionHandler
{
public:

	void Update(QuadTreeV2* quadTree);
	void Update(QuadTreeV3* quadTree);
	void Update(SweepAndPruneCollision* quadTree);

	void ClearCollisions();

	bool DetectCollision(std::shared_ptr<GameObjectDynamic> _nodeA, std::shared_ptr<GameObjectDynamic> _nodeB);

private:
	void UpdateCollisionPairs(collisionList list);

};

