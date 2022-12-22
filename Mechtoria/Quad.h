#pragma once
#include "Vec2.h"

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