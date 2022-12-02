#pragma once
#include "Node.h"
#include "Vec2.h"
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
	};
	CollisionVolume(CollisionVolumeConfiguration _params);
	~CollisionVolume() {};

	virtual void Initialization() override;
	virtual void Deinitialization() override {};
	virtual void Draw() override;

	void SetVisible(bool isVisible) { m_debugDraw = isVisible; };
	bool GetVisible() { return m_debugDraw; };

	void SetCollisionPair(std::weak_ptr<GameObjectDynamic> _pairs) { m_collisionPairs.push_back(_pairs); };
	const std::vector<std::weak_ptr<GameObjectDynamic>>& GetCollisionPairs() const { return m_collisionPairs; }
	void ClearCollisionPairs() { m_collisionPairs.clear(); }

	const CollisionVolumeConfiguration& GetConfirgutrtion() { return m_configuration; };
	void SetCollisionDims(Vec2<float> _dims) { m_configuration.dimentions = _dims; };
private:
	CollisionVolumeConfiguration m_configuration;
	bool m_debugDraw;
	std::vector<std::weak_ptr<GameObjectDynamic>> m_collisionPairs;
};

