#include "PhysicsHandler.h"
#include "CollisionVolume.h"
#include "GameObjectDynamic.h"
#include "QuadTreeV3.h"
#include <limits>

PhysicsHandler::PhysicsHandler(GameObjectDynamic* parent)
    :DynamicNode(node::type::PHYSICSHANDLER), 
    m_velocity({ 0.f,0.f }), 
    m_parent(parent),
    m_mass(1.f)
{
}

void PhysicsHandler::Update()
{
    float deltaTime = GetFrameTime();
    //Vec2<float> force{0.f,0.f};
    // apply gravity
    m_velocity += settings::physics::gravityVec * deltaTime;

    //actor collision
    const auto ActorList = GetParentCollisionVolume()->GetCollisionPairsRaw();
    //if (!ActorList.empty())
    //    m_velocity = { 0.f,0.f };
    
    // voxel collision
    auto voxelsList = GetParentCollisionVolume()->GetCollisionVoxels();
    if (!voxelsList.empty())
        CalculateVoxelCollisionV2(voxelsList);


    // set new actor position
    Transform2D transform = GetParentTransform();
    auto position = transform.GetPosition();
    position += CalculateAcceleration(m_velocity);
    transform.SetPosition(position);
    m_parent->SetWorldTransform(transform);
    m_parent->SetWorldTransform(transform);

}

void PhysicsHandler::AddInputForce(Vec2<float> force)
{
    if (force.GetY() < 0) // test Y force is not facter than current
    {
        if (m_velocity.GetY() > settings::physics::inputMinVec.GetY())
            m_velocity = { m_velocity.GetX(), force.GetY() };
    }
    else if (force.GetY() > 0)
    {
        if (m_velocity.GetY() < settings::physics::inputMaxVec.GetY())
            m_velocity = { m_velocity.GetX(), force.GetY() };
    }
    if (force.GetX() < 0)
    {
        if (m_velocity.GetX() > settings::physics::inputMinVec.GetX())
            m_velocity = { force.GetX(), m_velocity.GetY() };
    }
    else if (force.GetX() > 0)
    {
        if (m_velocity.GetX() < settings::physics::inputMaxVec.GetX())
            m_velocity = { force.GetX(), m_velocity.GetY() };
    }
}

void PhysicsHandler::AddPhysics(physics::component)
{
}

void PhysicsHandler::RemovePhysics(physics::component comp)
{
}

Transform2D PhysicsHandler::GetParentTransform() const
{
    return m_parent->GetWorldTransform();
}

CollisionVolume* PhysicsHandler::GetParentCollisionVolume()
{
    if (auto collisionNode = std::dynamic_pointer_cast<CollisionVolume>(m_parent->GetChildNode(node::type::COLLISION_2D_SHAPE).lock()))
    {
        return collisionNode.get();
    }
    else
        return nullptr;
}

Vec2<float> PhysicsHandler::CalculateAcceleration(Vec2<float> force)
{
    return force * m_mass;
}

void PhysicsHandler::CalculateVoxelCollision(std::vector<QuadTreeV3*>& voxels)
{
    auto volume = GetParentCollisionVolume();
    const auto volumePos = volume->GetWorldTransform().GetPosition();

    float volumeMaxX{0.f};
    float volumeMinX{0.f};
    float volumeMaxY{0.f};
    float volumeMinY{0.f};
    Transform2D transform = GetParentTransform();
    bool voxelAbove{ false };
    bool voxelBelow{ false }; 
    bool voxelLeft{ false };
    bool voxelRight{ false };
    bool verticalCollisionBelow{ false };
    bool verticalCollisionAbove{ false };
    bool horizontalCollisionLeft{ false };
    bool horizontalCollisionRight{ false };

    const auto volumeDims = volume->GetConfiguration().dimentions;
    // TODO: propper collision!!
    for (const auto& vox : voxels)
    {
        const auto voxTopLeft = vox->GetBounds().topLeft;
        const auto voxBotRight = vox->GetBounds().bottomRight;

        transform = GetParentTransform();
        volumeMinY = transform.GetPosition().GetY();
        volumeMaxY = transform.GetPosition().GetY() + volumeDims.GetY();
        voxelAbove = (volumeMinY >= voxTopLeft.GetY()) && (volumeMinY <= voxBotRight.GetY());
        voxelBelow = (volumeMaxY >= voxTopLeft.GetY()) && (volumeMaxY <= voxBotRight.GetY());

        verticalCollisionBelow = (voxelBelow && m_velocity.GetY() > 0.f);
        verticalCollisionAbove = (voxelAbove && m_velocity.GetY() < 0.f);
        if (verticalCollisionBelow || verticalCollisionAbove)
        {
            m_velocity = { m_velocity.GetX(), 0.f };
            if(verticalCollisionBelow)
                transform.SetPosition({ GetParentTransform().GetPosition().GetX() , voxTopLeft.GetY() - volumeDims.GetY()});
            else if (verticalCollisionAbove)
                transform.SetPosition({ GetParentTransform().GetPosition().GetX() , voxBotRight.GetY() });
            m_parent->SetWorldTransform(transform);
        }

        //transform = GetParentTransform();
        //volumeMinX = transform.GetPosition().GetX();
        //volumeMaxX = transform.GetPosition().GetX() + volumeDims.GetX();
        //voxelLeft = (volumeMinX >= voxTopLeft.GetX()) && (volumeMinX <= voxBotRight.GetX());
        //voxelRight = (volumeMaxX >= voxTopLeft.GetX()) && (volumeMaxX <= voxBotRight.GetX());

        //horizontalCollisionLeft = (voxelLeft && m_velocity.GetX() < 0.f);
        //horizontalCollisionRight = (voxelRight && m_velocity.GetX() > 0.f);
        //if ( horizontalCollisionLeft || horizontalCollisionRight)
        //{
        //    m_velocity = { 0.f, m_velocity.GetY() };
        //    transform = GetParentTransform();
        //    /*if(horizontalCollisionLeft)
        //        transform.SetPosition({ vox->GetBounds().bottomRight.GetX(), transform.GetPosition().GetY() });
        //    if(horizontalCollisionRight)
        //        transform.SetPosition({ vox->GetBounds().topLeft.GetX() - volumeDims.GetX(), transform.GetPosition().GetY() });*/
        //    m_parent->SetWorldTransform(transform);
        //}
    }
}

void PhysicsHandler::CalculateVoxelCollisionV2(std::vector<QuadTreeV3*>& voxels)
{
    auto volume = GetParentCollisionVolume();
    const auto volumeDims = volume->GetConfiguration().dimentions;
    float lastBottomHeight = std::numeric_limits<float>::max();
    auto tempoVelocity = m_velocity;
    for (const auto& vox : voxels)
    {
        const auto voxTopLeft = vox->GetBounds().topLeft;
        const auto voxBotRight = vox->GetBounds().bottomRight;

        const auto flag = volume->TestBlockingCollision(vox->GetBounds());
        if (flag == (int)physics::directionBlock::NO_DIRECTION)
            continue;

        auto transform = GetParentTransform();
        if (flag & (int)physics::directionBlock::SOUTH)
        {
            if (m_velocity.GetY() > 0.f)
            {
                if (voxTopLeft.GetY() < lastBottomHeight)
                    lastBottomHeight = voxTopLeft.GetY();
                tempoVelocity = { tempoVelocity.GetX(), 0.f };
                transform.SetPosition({ GetParentTransform().GetPosition().GetX() , lastBottomHeight - volumeDims.GetY() });
                m_parent->SetWorldTransform(transform);
                transform = GetParentTransform();
            }
        }
        if (flag & (int)physics::directionBlock::EAST)
        {
            if (m_velocity.GetX() > 0.f)
            {
                tempoVelocity = { 0.f, tempoVelocity.GetY() };
                transform.SetPosition({ voxTopLeft.GetX() - volumeDims.GetX(), GetParentTransform().GetPosition().GetY() });
                m_parent->SetWorldTransform(transform);
                transform = GetParentTransform();
            }
        }
        if (flag & (int)physics::directionBlock::WEST)
        {
            if (m_velocity.GetX() < 0.f)
            {
                tempoVelocity = { 0.f, tempoVelocity.GetY() };
                transform.SetPosition({ voxBotRight.GetX(), GetParentTransform().GetPosition().GetY() });
                m_parent->SetWorldTransform(transform);
                transform = GetParentTransform();
            }
        }
        if (flag & (int)physics::directionBlock::NORTH)
        {
            if (m_velocity.GetY() < 0.f)
            {
                tempoVelocity = { tempoVelocity.GetX(), 0.f };
                transform.SetPosition({ GetParentTransform().GetPosition().GetX() , voxBotRight.GetY() });
                m_parent->SetWorldTransform(transform);
                transform = GetParentTransform();
            }
        }
    }
    m_velocity = tempoVelocity;
}
