#pragma once
#include "Node.h"
#include "Vec2.h"
#include "Transform.h"
#include "raylibcpp.h"
class Shape : public Visual2DDynamicNode 
{
public:
	enum class ShapeType
	{
		rectangle,
		rectangleBoader,
		circle
	};
	struct ShapeConfiguration
	{
		ShapeType type = ShapeType::rectangle;
		Vec2<float> dimentions{10.f,10.f};
		Color colour = BLUE;
		float lineWidth = 1;
		std::weak_ptr<RendableNode> parent;
	};
	Shape(ShapeConfiguration _params);
	~Shape() {};

	virtual void Initialization() override {};
	virtual void Deinitialization() override {};
	virtual void Draw() override;
private:
	ShapeConfiguration m_configuration;
};

