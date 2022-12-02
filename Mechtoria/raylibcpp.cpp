#include "raylibcpp.h"
#include <assert.h>

void raycpp::DrawRectangle(Vec2<float> _position, Vec2<float> _dims, Color _colour)
{
	assert(_position.GetX() >= 0 && _position.GetX() < GetScreenWidth()); // x is in range
	assert(_position.GetY() >= 0 && _position.GetY() < GetScreenHeight()); // y is in range
	DrawRectangle((int)_position.GetX(), (int)_position.GetY(), (int)_dims.GetX(), (int)_dims.GetY(), _colour);
}

void raycpp::DrawRectangleLinesEx(Vec2<float> _pos, Vec2<float> _dims, float _lineThickness, Color _color)
{
	assert(_pos.GetX() >= 0 && _pos.GetX() <= GetScreenWidth()); // x is in range
	assert(_pos.GetY() >= 0 && _pos.GetY() <= GetScreenHeight()); // y is in range
	assert(_lineThickness > 0); // lines is at lease 1 thickness
	DrawRectangleLinesEx({_pos.GetX(), _pos.GetY(), _dims.GetX(), _dims.GetY()}, _lineThickness, _color);
}

void raycpp::DrawCircle(Vec2<float> _pos, float _radius, Color _color)
{
	assert(_pos.GetX() >= 0 && _pos.GetX() <= GetScreenWidth()); // x is in range
	assert(_pos.GetY() >= 0 && _pos.GetY() <= GetScreenHeight()); // y is in range
	DrawCircle((int)_pos.GetX(), (int)_pos.GetY(), _radius, _color);
}