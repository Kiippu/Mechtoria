#pragma once
#include "Vec2.h"
#include <memory>
#include <vector>

#include "Node.h"

struct Quad
{
	Quad(Vec2<float> topLeft_, Vec2<float> dims_)
		: topLeft(topLeft_), dimentions(dims_), bottomRight(topLeft_ + dims_)
	{}
	const Vec2<float> topLeft;
	const Vec2<float> bottomRight;
	const Vec2<float> dimentions;
	Vec2<float> getMidPoint() const
	{
		float x = { topLeft.GetX() + ((bottomRight.GetX() - topLeft.GetX()) / 2) };
		float y = { topLeft.GetY() + ((bottomRight.GetY() - topLeft.GetY()) / 2) };
		return Vec2<float>{ x, y };
	}
	bool Contains(Vec2<float> point)
	{
		const bool isMoreThanTopLeft = (point.GetX() > topLeft.GetX() && point.GetY() > topLeft.GetY());
		const bool isLessThanBotRight = (point.GetX() < bottomRight.GetX() && point.GetY() < bottomRight.GetY());
		return (isMoreThanTopLeft && isLessThanBotRight);
	}
	bool Intersects(Quad range)
	{
		return !(range.topLeft.GetX() > bottomRight.GetX() ||
				range.bottomRight.GetX() < topLeft.GetX() ||
				range.topLeft.GetY() > bottomRight.GetY() ||
				range.bottomRight.GetY() < topLeft.GetY());
	}
};

class QuadTreeV2 : RendableNode
{
public:
	QuadTreeV2(Quad boundry, int capacity)
		: RendableNode(node::renderLayer::DEBUG_NON_GUI), 
		m_boundry(boundry), 
		m_capacity(capacity), 
		m_isDivided(false)
	{
		m_points.reserve(m_capacity);
	}

	virtual void Draw() override;

	bool Insert(Vec2<float> point);
	void Query(Quad range, std::vector<Vec2<float>>& pointsFound);

private:
	Quad m_boundry;
	const int m_capacity;
	// subdivisions
	std::unique_ptr<QuadTreeV2> m_northWest;
	std::unique_ptr<QuadTreeV2> m_northEast;
	std::unique_ptr<QuadTreeV2> m_southWest;
	std::unique_ptr<QuadTreeV2> m_southEast;
	// state
	bool m_isDivided;
	std::vector<Vec2<float>> m_points;
private:
	void Subdivide();

};