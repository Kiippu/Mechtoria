#pragma once
#include "Node.h"
#include "Vec2.h"
#include "Quad.h"

class QuadTreeV3;

class CollisionVolume : public Visual2DDynamicNode
{

public:
	enum class VolumeType
	{
		rectangle,
		circle
	};
	struct CollisionVolumeConfiguration
	{
		VolumeType type = VolumeType::rectangle;
		Vec2<float> dimentions{ 100.f,100.f };
		Color onCollisionColour = RED;
		Color onNoCollisionColour = BLACK;
		float borderWidth = 5;
		state::type renderState = state::type::SKIP;
	};
	CollisionVolume(CollisionVolumeConfiguration _params);
	~CollisionVolume() {};

	virtual void Initialization() override;
	virtual void Draw() override;
	virtual void Update() override;

	void SetVisible(bool isVisible) { m_debugDraw = isVisible; };
	bool GetVisible() { return m_debugDraw; };

	//void SetCollisionPair(std::weak_ptr<GameObjectDynamic> _pairs) { m_collisionPairs.push_back(_pairs); };
	void SetCollisionPair(std::vector<GameObjectDynamic*> _pairs);
	void AddCollisionPair(GameObjectDynamic* obj);

	void SetCollisionVoxel(std::vector<QuadTreeV3*> voxels);

	//const std::vector<std::weak_ptr<GameObjectDynamic>>& GetCollisionPairs() const { return m_collisionPairs; }
	const std::vector<GameObjectDynamic*>& GetCollisionPairsRaw() const { return m_collisionPairsRaw; }
	const std::vector<QuadTreeV3*>& GetCollisionVoxels() const { return m_collisionVoxels; }

	const Quad* GetBlockingCollisionQuad(physics::directionBlock side);
	// return flag of collsions hit
	const int TestBlockingCollision(const Quad& quad);

	void ClearCollisionPairs();

	const CollisionVolumeConfiguration& GetConfiguration() { return m_configuration; };
	void SetCollisionDims(Vec2<float> _dims) { m_configuration.dimentions = _dims; };
private:
	CollisionVolumeConfiguration m_configuration;
	bool m_debugDraw;
	std::vector<std::weak_ptr<GameObjectDynamic>> m_collisionPairs;
	std::vector<GameObjectDynamic*> m_collisionPairsRaw;
	std::vector<QuadTreeV3*> m_collisionVoxels;
	std::map<physics::directionBlock, Quad> m_blockingCollision;

private:
	void DrawBlockingCollision(physics::directionBlock side, const int& flag);
};

