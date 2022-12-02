#include "Shape.h"
#include "Setting.h"

Shape::Shape(ShapeConfiguration _params)
	: m_configuration(_params), Visual2DDynamicNode(node::type::VISUAL_2D_SHAPE, node::renderLayer::FOREGROUND)
{

}

void Shape::Draw()
{
	if (auto parent = m_configuration.parent.lock())
	{
		switch (m_configuration.type)
		{
		case Shape::ShapeType::rectangle:
			raycpp::DrawRectangle(parent->GetWorldTransform().GetPosition(), m_configuration.dimentions, m_configuration.colour);
			break;
		case Shape::ShapeType::rectangleBoader:
			raycpp::DrawRectangleLinesEx(parent->GetWorldTransform().GetPosition(), m_configuration.dimentions, m_configuration.lineWidth, m_configuration.colour);
			break;
		case Shape::ShapeType::circle:
			raycpp::DrawCircle(parent->GetWorldTransform().GetPosition(), m_configuration.dimentions.GetX(), m_configuration.colour);
		default:
			break;
		}
	}
}
