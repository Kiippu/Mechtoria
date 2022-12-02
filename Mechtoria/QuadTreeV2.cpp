#include "QuadTreeV2.h"
#include "raylibcpp.h"

void QuadTreeV2::Draw()
{
    raycpp::DrawRectangleLinesEx(m_boundry.topLeft, m_boundry.dimentions, 1.f, BLACK);
    for (auto& point : m_points)
        raycpp::DrawCircle(point, 2.f, RED);
    if(m_northWest) m_northWest->Draw();
    if(m_northEast) m_northEast->Draw();
    if(m_southEast) m_southEast->Draw();
    if(m_southWest) m_southWest->Draw();
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

void QuadTreeV2::Subdivide()
{
    const Vec2<float>& TOP_LEFT = m_boundry.topLeft;
    Vec2<float> HALF_DIMS = m_boundry.dimentions * .5f;
    Vec2<float> TOP_MID = TOP_LEFT + Vec2<float>{HALF_DIMS.GetX(), 0.f};
    Vec2<float> MID_LEFT = TOP_LEFT + Vec2<float>{0.f, HALF_DIMS.GetY()};
    Vec2<float> MID_MID = TOP_LEFT + HALF_DIMS;

    m_northWest = std::make_unique<QuadTreeV2>(Quad{ TOP_LEFT,  HALF_DIMS }, m_capacity);
	m_northEast = std::make_unique<QuadTreeV2>(Quad{ TOP_MID,   HALF_DIMS }, m_capacity);
	m_southWest = std::make_unique<QuadTreeV2>(Quad{ MID_LEFT,  HALF_DIMS }, m_capacity);
	m_southEast = std::make_unique<QuadTreeV2>(Quad{ MID_MID,   HALF_DIMS }, m_capacity);
    m_isDivided = true;
}
