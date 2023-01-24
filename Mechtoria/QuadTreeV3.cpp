#include "QuadTreeV3.h"
#include "raylibcpp.h"

void QuadTreeV3::Draw()
{
    DrawVoxels();
    if (settings::DEBUG_QUADTREE)
        DrawDebugQuad();
}

bool QuadTreeV3::Insert(GameObjectDynamic* obj, CollisionVolume* vol)
{
    if (!vol && obj)
    {
        if (const auto collisionNode = std::dynamic_pointer_cast<CollisionVolume>(obj->GetChildNode(node::type::COLLISION_2D_SHAPE).lock()))
        {
            vol = collisionNode.get();
        }
    }
    Quad range{ vol->GetWorldTransform().GetPosition(), vol->GetConfiguration().dimentions };
    if (!m_boundry.Intersects(range))
        return false;
    if (m_points.size() < m_capacity)
    {
        m_points.push_back(obj);
        return true;
    }
    else
    {
        if (!m_isDivided)
        {
            Subdivide();
        }
        if (m_northWest->Insert(obj, vol))
            return true;
        else if (m_northEast->Insert(obj, vol))
            return true;
        else if (m_southWest->Insert(obj, vol))
            return true;
        else if (m_southEast->Insert(obj, vol))
            return true;
    }
    return false;
}

void QuadTreeV3::Query(Quad range, std::vector<GameObjectDynamic*>& pointsFound)
{
    if (!m_boundry.Intersects(range))
        return;
    else
    {
        for (const auto& obj : m_points)
        {
            if (const auto collisionNode = std::dynamic_pointer_cast<CollisionVolume>(obj->GetChildNode(node::type::COLLISION_2D_SHAPE).lock()))
            {
                Quad rangeObj{ collisionNode->GetWorldTransform().GetPosition(), collisionNode->GetConfiguration().dimentions };
                if ((rangeObj.topLeft != range.topLeft) && range.Intersects(rangeObj))
                    pointsFound.push_back(obj);
            }
        }

        if (!m_isDivided) return;
        if (m_northEast) m_northEast->Query(range, pointsFound);
        if (m_northWest) m_northWest->Query(range, pointsFound);
        if (m_southEast) m_southEast->Query(range, pointsFound);
        if (m_southWest) m_southWest->Query(range, pointsFound);
    }
}

void QuadTreeV3::ClearInserts()
{
    if (m_voxelState > 0)
    {
        m_points.clear();
        if (m_northEast) m_northEast.reset();
        if (m_northWest) m_northWest.reset();
        if (m_southEast) m_southEast.reset();
        if (m_southWest) m_southWest.reset();
        m_isDivided = false;
        return;
    }
    if (m_voxelState == 0)
    {
        m_points.clear();
        if (m_northEast) m_northEast->ClearInserts();
        if (m_northWest) m_northWest->ClearInserts();
        if (m_southEast) m_southEast->ClearInserts();
        if (m_southWest) m_southWest->ClearInserts();

        if (m_northEast && m_northEast->m_points.empty() && m_northEast->m_voxelState == 0 && !m_northEast->m_northEast &&
            m_northWest && m_northWest->m_points.empty() && m_northWest->m_voxelState == 0 && !m_northWest->m_northWest &&
            m_southEast && m_southEast->m_points.empty() && m_southEast->m_voxelState == 0 && !m_southEast->m_southEast &&
            m_southWest && m_southWest->m_points.empty() && m_southWest->m_voxelState == 0 && !m_southWest->m_southWest)
        {
            m_northEast.reset();
            m_northWest.reset();
            m_southEast.reset();
            m_southWest.reset();
            m_isDivided = false;
        }
    }

    // TODO: remove all actors i quadtree
}

void QuadTreeV3::QueryVoxels(Quad range, std::vector<QuadTreeV3*>& voxelsFound, int depth)
{
    (depth == INT_MIN) ? depth = 0 : ++depth;
    if (!m_boundry.Intersects(range))
        return;
    else
    {
        if (depth == m_voxelDepth)
        {
            if (m_voxelState > 0)
            {
                voxelsFound.push_back(this);
                return;
            }
        }
        else
        {
            if (!m_isDivided) return;
            if (m_northEast) m_northEast->QueryVoxels(range, voxelsFound, depth);
            if (m_northWest) m_northWest->QueryVoxels(range, voxelsFound, depth);
            if (m_southEast) m_southEast->QueryVoxels(range, voxelsFound, depth);
            if (m_southWest) m_southWest->QueryVoxels(range, voxelsFound, depth);
        }
    }
}

void QuadTreeV3::InsertVoxels(Quad range, int depth)
{
    (depth == INT_MIN) ? depth = 0 : ++depth;
    if (!m_boundry.Intersects(range))
        return;
    else
    {
        if (depth < m_voxelDepth)
        {
            if (!m_isDivided)
            {
                Subdivide((depth == m_voxelDepth - 1));
            }
            if (m_northEast) m_northEast->InsertVoxels(range, depth);
            if (m_northWest) m_northWest->InsertVoxels(range, depth);
            if (m_southEast) m_southEast->InsertVoxels(range, depth);
            if (m_southWest) m_southWest->InsertVoxels(range, depth);
        }
        else
        {
            Vec2<float> northEast = { m_boundry.topLeft + Vec2<float>{m_boundry.dimentions.GetX(), 0.f} };
            Vec2<float> southWest = { m_boundry.bottomRight - Vec2<float>{m_boundry.dimentions.GetX(), 0.f} };
            if (raycpp::CheckCollisionPointRec(m_boundry.topLeft, range))
                m_voxelState |= (int)voxel::Mask::NORTH_WEST;
            if (raycpp::CheckCollisionPointRec(northEast, range))
                m_voxelState |= (int)voxel::Mask::NORTH_EAST;
            if (raycpp::CheckCollisionPointRec(southWest, range))
                m_voxelState |= (int)voxel::Mask::SOUTH_WEST;
            if (raycpp::CheckCollisionPointRec(m_boundry.bottomRight, range))
                m_voxelState |= (int)voxel::Mask::SOUTH_EAST;
        }
        if (AreChildrenActive())
        {
            m_voxelState = 0xf;
        }
    }
}

void QuadTreeV3::RemoveVoxels(Quad range, int depth)
{
    (depth == INT_MIN) ? depth = 0 : ++depth;
    if (!m_boundry.Intersects(range))
        return;
    else
    {
        if (depth < m_voxelDepth)
        {
            if (m_voxelState == 0xf)
            {
                m_voxelState = m_voxelState;
                Subdivide(false);
                m_northEast->m_voxelState = 0xf;
                m_northWest->m_voxelState = 0xf;
                m_southEast->m_voxelState = 0xf;
                m_southWest->m_voxelState = 0xf;
            }
            if (m_northEast) {
                m_northEast->RemoveVoxels(range, depth);
                if (!m_northEast->AreChildrenActive())
                    m_voxelState &= ~(int)voxel::Mask::NORTH_EAST;
            }
            if (m_northWest) {
                m_northWest->RemoveVoxels(range, depth);
                if (!m_northWest->AreChildrenActive())
                    m_voxelState &= ~(int)voxel::Mask::NORTH_WEST;
            }
            if (m_southEast) {
                m_southEast->RemoveVoxels(range, depth);
                if (!m_southEast->AreChildrenActive())
                    m_voxelState &= ~(int)voxel::Mask::SOUTH_EAST;
            }
            if (m_southWest) {
                m_southWest->RemoveVoxels(range, depth);
                if (!m_southWest->AreChildrenActive())
                    m_voxelState &= ~(int)voxel::Mask::SOUTH_WEST;
            }
            
        }
        else
        {
            Vec2<float> northWest = { m_boundry.topLeft + Vec2<float>{m_boundry.dimentions.GetX(), 0.f} };
            Vec2<float> southEast = { m_boundry.bottomRight - Vec2<float>{m_boundry.dimentions.GetX(), 0.f} };
            if (raycpp::CheckCollisionPointRec(m_boundry.topLeft, range))
                m_voxelState &= ~(int)voxel::Mask::NORTH_WEST;
            if (raycpp::CheckCollisionPointRec(northWest, range))
                m_voxelState &= ~(int)voxel::Mask::NORTH_EAST;
            if (raycpp::CheckCollisionPointRec(m_boundry.bottomRight, range))
                m_voxelState &= ~(int)voxel::Mask::SOUTH_WEST;
            if (raycpp::CheckCollisionPointRec(southEast, range))
                m_voxelState &= ~(int)voxel::Mask::SOUTH_EAST;
        }
    }
}

bool QuadTreeV3::AreChildrenActive() const
{
    return (
        m_northEast && m_northWest && m_southEast && m_southWest &&
        m_northEast->m_voxelState == 0xf &&
        m_northWest->m_voxelState == 0xf &&
        m_southEast->m_voxelState == 0xf &&
        m_southWest->m_voxelState == 0xf
        );
}

void QuadTreeV3::Subdivide(bool isVoxel)
{
    const Vec2<float>& TOP_LEFT = m_boundry.topLeft;
    Vec2<float> HALF_DIMS = m_boundry.dimentions * .5f;
    Vec2<float> TOP_MID = TOP_LEFT + Vec2<float>{HALF_DIMS.GetX(), 0.f};
    Vec2<float> MID_LEFT = TOP_LEFT + Vec2<float>{0.f, HALF_DIMS.GetY()};
    Vec2<float> MID_MID = TOP_LEFT + HALF_DIMS;
    node::renderLayer layer = isVoxel ? node::renderLayer::VOXEL : node::renderLayer::SKIP_NODE;

    m_northWest = std::make_unique<QuadTreeV3>(Quad{ TOP_LEFT,  HALF_DIMS }, m_capacity, m_ownerState, m_voxelDepth, layer);
    m_northEast = std::make_unique<QuadTreeV3>(Quad{ TOP_MID,   HALF_DIMS }, m_capacity, m_ownerState, m_voxelDepth, layer);
    m_southWest = std::make_unique<QuadTreeV3>(Quad{ MID_LEFT,  HALF_DIMS }, m_capacity, m_ownerState, m_voxelDepth, layer);
    m_southEast = std::make_unique<QuadTreeV3>(Quad{ MID_MID,   HALF_DIMS }, m_capacity, m_ownerState, m_voxelDepth, layer);
    m_isDivided = true;
}

void QuadTreeV3::DrawDebugQuad()
{
    raycpp::DrawRectangleLinesEx(m_boundry.topLeft, m_boundry.dimentions, 3.f, BLACK);
    //for (auto& obj : m_points)
    //{
    //    if (const auto collisionNode = std::dynamic_pointer_cast<CollisionVolume>(obj->GetChildNode(node::type::COLLISION_2D_SHAPE).lock()))
    //        raycpp::DrawRectangleLinesEx(collisionNode->GetWorldTransform().GetPosition(), collisionNode->GetConfiguration().dimentions, 2.f, YELLOW);
    //    //raycpp::DrawCircle(point, 2.f, RED);
    //}

    if (m_northWest) m_northWest->Draw();
    if (m_northEast) m_northEast->Draw();
    if (m_southEast) m_southEast->Draw();
    if (m_southWest) m_southWest->Draw();
}

void QuadTreeV3::DrawVoxels()
{
    if (m_voxelState != (int)voxel::Mask::NO_VOXEL)
    {
        if ( m_voxelState == 0xf || ( GetRenderLayer() == node::renderLayer::VOXEL && m_voxelState))
        {
            raycpp::DrawRectangle(m_boundry.topLeft, m_boundry.dimentions, DARKGREEN);
        }
    }
    else
    {
        if (m_northWest) m_northWest->Draw();
        if (m_northEast) m_northEast->Draw();
        if (m_southEast) m_southEast->Draw();
        if (m_southWest) m_southWest->Draw();
    }
}

void QuadTreeV3::PruneTree()
{
    //bool nw = (m_northWest && !isChildValid(m_northWest.get()));
    ////    //m_northWest.reset();
    //bool ne = (m_northEast && !isChildValid(m_northEast.get()));
    ////    //m_northEast.reset();
    //bool sw = (m_southWest && !isChildValid(m_southWest.get()));
    ////    //m_southWest.reset();
    //bool se = (m_southEast && !isChildValid(m_southEast.get()));
    ////    //m_southEast.reset();
    //if (!se && !sw && !nw && !ne && m_points.empty() && m_voxelState == 0)// && m_voxelState == (int)voxel::Mask::NO_VOXEL)
    //    if(m_points.empty() && m_voxelState == 0)
    //    {
    //        m_northWest.reset();
    //        m_northEast.reset();
    //        m_southWest.reset();
    //        m_southEast.reset();
    //        m_isDivided = false;
    //    }

    /*if (!m_northWest && !m_northEast && !m_southWest && !m_southEast)
        m_isDivided = false;*/
}

bool QuadTreeV3::isChildValid(QuadTreeV3* child)
{
    return (!child->m_points.empty()
        || child->m_northWest
        || child->m_northEast
        || child->m_southWest
        || child->m_southEast
        || child->m_voxelState != (int)voxel::Mask::NO_VOXEL);
}
