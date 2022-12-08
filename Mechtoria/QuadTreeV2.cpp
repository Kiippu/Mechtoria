#include "QuadTreeV2.h"
#include "raylibcpp.h"

void QuadTreeV2::Draw()
{
    if(m_voxelState != (int)voxel::Mask::NO_VOXEL)
    {
        DrawVoxels();
    }
    if (settings::DEBUG_QUADTREE)
        DrawDebugQuad();
}

bool QuadTreeV2::Insert(Vec2<float> point)
{
    if (!m_boundry.Contains(point))
        return false;
    if (m_points.size() < m_capacity)
    {
        m_points.push_back(point);
        return true;
    }
    else
    {
        if (!m_isDivided)
        {
            Subdivide();
        }
        if (m_northWest->Insert(point))
            return true;
        else if(m_northEast->Insert(point))
            return true;
        else if (m_southWest->Insert(point))
            return true;
        else if (m_southEast->Insert(point))
            return true;
    }
    return false;
}

void QuadTreeV2::Query(Quad range, std::vector<Vec2<float>>& pointsFound)
{
    if (!m_boundry.Intersects(range))
        return;
    else
    {
        for (const auto& point : m_points)
        {
            if (range.Contains(point))
                pointsFound.push_back(point);
        }

        if (!m_isDivided) return;
        if (m_northEast) m_northEast->Query(range, pointsFound);
        if (m_northWest) m_northWest->Query(range, pointsFound);
        if (m_southEast) m_southEast->Query(range, pointsFound);
        if (m_southWest) m_southWest->Query(range, pointsFound);
    }
}

void QuadTreeV2::InsertVoxels(Quad range, int depth)
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
                Subdivide();
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
            if (raycpp::CheckCollisionPointRec( m_boundry.topLeft, range))
                m_voxelState |= (int)voxel::Mask::NORTH_WEST;
            if (raycpp::CheckCollisionPointRec(northEast, range))
                m_voxelState |= (int)voxel::Mask::NORTH_EAST;
            if (raycpp::CheckCollisionPointRec( southWest, range))
                m_voxelState |= (int)voxel::Mask::SOUTH_WEST;
            if (raycpp::CheckCollisionPointRec( m_boundry.bottomRight, range))
                m_voxelState |= (int)voxel::Mask::SOUTH_EAST;
        }
    }
}

void QuadTreeV2::RemoveVoxels(Quad range, int depth)
{
    (depth == INT_MIN) ? depth = 0 : ++depth;
    if (!m_boundry.Intersects(range))
        return;
    else
    {
        if (depth < m_voxelDepth)
        {
            if (m_northEast) m_northEast->RemoveVoxels(range, depth);
            if (m_northWest) m_northWest->RemoveVoxels(range, depth);
            if (m_southEast) m_southEast->RemoveVoxels(range, depth);
            if (m_southWest) m_southWest->RemoveVoxels(range, depth);
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
        // TODO: not pruning currect tiles needs rework
        //PruneTree();
    }
}

void QuadTreeV2::Subdivide(bool isVoxel)
{
    const Vec2<float>& TOP_LEFT = m_boundry.topLeft;
    Vec2<float> HALF_DIMS = m_boundry.dimentions * .5f;
    Vec2<float> TOP_MID = TOP_LEFT + Vec2<float>{HALF_DIMS.GetX(), 0.f};
    Vec2<float> MID_LEFT = TOP_LEFT + Vec2<float>{0.f, HALF_DIMS.GetY()};
    Vec2<float> MID_MID = TOP_LEFT + HALF_DIMS;
    node::renderLayer layer = isVoxel ? node::renderLayer::VOXEL : node::renderLayer::SKIP_NODE;

    m_northWest = std::make_unique<QuadTreeV2>(Quad{ TOP_LEFT,  HALF_DIMS }, m_capacity, m_voxelDepth, layer);
	m_northEast = std::make_unique<QuadTreeV2>(Quad{ TOP_MID,   HALF_DIMS }, m_capacity, m_voxelDepth, layer);
	m_southWest = std::make_unique<QuadTreeV2>(Quad{ MID_LEFT,  HALF_DIMS }, m_capacity, m_voxelDepth, layer);
	m_southEast = std::make_unique<QuadTreeV2>(Quad{ MID_MID,   HALF_DIMS }, m_capacity, m_voxelDepth, layer);
    m_isDivided = true;
}

void QuadTreeV2::DrawDebugQuad()
{
    raycpp::DrawRectangleLinesEx(m_boundry.topLeft, m_boundry.dimentions, 1.f, BLACK);
    for (auto& point : m_points)
        raycpp::DrawCircle(point, 2.f, RED);
    if (m_northWest) m_northWest->Draw();
    if (m_northEast) m_northEast->Draw();
    if (m_southEast) m_southEast->Draw();
    if (m_southWest) m_southWest->Draw();
}

void QuadTreeV2::DrawVoxels()
{
    if (m_voxelState)
    {
        if (settings::voxelType == voxel::DrawType::CUBIC)
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

void QuadTreeV2::PruneTree()
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

bool QuadTreeV2::isChildValid(QuadTreeV2* child)
{
    return (!child->m_points.empty()
        || child->m_northWest
        || child->m_northEast
        || child->m_southWest
        || child->m_southEast
        || child->m_voxelState != (int)voxel::Mask::NO_VOXEL);
}
