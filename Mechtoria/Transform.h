#pragma once
#include <raylib.h>
#include "Vec2.h"

class Transform2D
{
public:
	struct TransformConfiguration
	{
		Vec2<float> position;
		float rotation;
		float scale;
	};

	//Transform2D(Transform2D& _engine) = default;
	Transform2D(const Transform2D& _engine)
		: m_position(_engine.GetPosition()), m_rotation(_engine.GetRotation()), m_scale(_engine.GetScale())
	{};
	constexpr Transform2D& operator=(const Transform2D& _rhs) = default;
	Transform2D()
		: m_position(Vec2<float>{0, 0}), m_rotation(0.f), m_scale(1.f)
	{};
	Transform2D(Vec2<float> _pos, float _rot, float _scale)
		: m_position(_pos), m_rotation(_rot), m_scale(_scale)
	{};
	~Transform2D() = default;

	Vec2<float> GetPosition() const { return m_position; };
	Vec2<float>* GetPositionPtr() { return &m_position; };
	Vector2 GetPosition_RayLib() const { return { m_position.GetX(), m_position.GetY()}; };
	void SetPosition(Vec2<float> _pos) { m_position = _pos; };

	float GetRotation() const { return m_rotation; };
	void SetRotation(float _rot) { m_rotation = _rot; }

	float GetScale() const { return m_scale; };
	void SetScale(float _scale) { m_scale = _scale; }

private:
	Vec2<float> m_position;
	float m_rotation;
	float m_scale;
};