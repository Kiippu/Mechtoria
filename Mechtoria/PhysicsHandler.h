#pragma once
#include <vector>
#include "Node.h"
#include "Vec2.h"
#include "Setting.h"

class CollisionVolume;
class GameObjectDynamic;
class QuadTreeV3;
struct Transform;

class PhysicsHandler :
    public DynamicNode
{
public:
    PhysicsHandler(GameObjectDynamic* parent);

    virtual void Update() override;

    void AddInputForce(Vec2<float> force);

    void AddPhysics(physics::component comp);
    void RemovePhysics(physics::component comp);

private:

    GameObjectDynamic* m_parent;
    Vec2<float> m_velocity;
    float m_mass;
    int m_blockMovementDirectionFlags;
private:

    Transform2D GetParentTransform() const;
    CollisionVolume* GetParentCollisionVolume();

    // Calculate Newtons Second Law
    Vec2<float> CalculateAcceleration(Vec2<float> force);
    void CalculateVoxelCollision(std::vector<QuadTreeV3*>& voxels);
    void CalculateVoxelCollisionV2(std::vector<QuadTreeV3*>& voxels);
};

