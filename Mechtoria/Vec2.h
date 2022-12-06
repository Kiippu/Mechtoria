#pragma once
#include "raylib.h"

template<class T>
class Vec2
{
public:
	Vec2() = default;
	constexpr Vec2(T _x, T _y)
		: x(_x), y(_y)
	{}

	constexpr T GetX() const { return x; };
	constexpr T GetY() const { return y; };
	constexpr void SetX(T _x) { x = _x; };
	constexpr void SetY(T _y) { y = _y; };
	constexpr const Vector2 AsVector2() { return Vector2{ x,y }; };


public:
	constexpr bool operator==(const Vec2& rhs) const
	{
		return (x == rhs.x && y == rhs.y);
	}
	constexpr bool operator!=(const Vec2& rhs) const
	{
		return !(*this == rhs);
	}
	constexpr Vec2 operator+(const Vec2& _rhs) const
	{
		return { x + _rhs.x, y + _rhs.y };
	}
	constexpr Vec2 operator+(const int rhs) const
	{
		return { x + rhs, y + rhs };
	}
	constexpr Vec2 operator+(const float rhs) const
	{
		return { x + rhs, y + rhs };
	}
	constexpr Vec2& operator+=(const Vec2& rhs)
	{
		return *this = *this + rhs;
	}
	constexpr Vec2& operator-=(const Vec2& rhs)
	{
		return *this = *this - rhs;
	}
	constexpr Vec2& operator*=(const Vec2& rhs)
	{
		return *this = *this * rhs;
	}
	constexpr Vec2 operator-(const Vec2& _rhs) const
	{
		return { x - _rhs.x, y - _rhs.y };
	}
	constexpr Vec2 operator-(const int _rhs) const
	{
		return { x - _rhs, y - _rhs };
	}
	constexpr Vec2 operator-(const float _rhs) const
	{
		return { x - _rhs, y - _rhs };
	}
	constexpr Vec2 operator*(const Vec2& _rhs) const
	{
		return { x * _rhs.x, y * _rhs.y };
	}
	constexpr Vec2 operator*(const int _rhs) const
	{
		return { x * _rhs, y * _rhs };
	}
	constexpr Vec2 operator*(const float _rhs) const
	{
		return { x * _rhs, y * _rhs };
	}

private:
	T x;
	T y;
};