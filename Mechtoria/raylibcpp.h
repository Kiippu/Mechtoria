#pragma once

#include <raylib.h>
#include "Vec2.h"
#include "QuadTreeV2.h"

namespace raycpp
{
	void DrawRectangle(Vec2<float> _position, Vec2<float> _dimentions, Color color);
	void DrawRectangleLinesEx(Vec2<float> _position, Vec2<float> _dimentions, float _lineThickness, Color _color);
	void DrawCircle(Vec2<float> _pos, float _radius, Color _color);
	void DrawLineEx(Vec2<float> startPos, Vec2<float> endPos, float thick, Color color);

	// collision
	bool CheckCollisionPointRec(Vec2<float> point, Quad rec);
}