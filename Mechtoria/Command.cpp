#include "Command.h"
#include "PhysicsHandler.h"

void MoveRightCommand::Execute(GameObjectDynamic* _obj)
{
	auto comp = _obj->GetChildNode(node::type::PHYSICSHANDLER);
	if (auto physics = comp.lock())
	{
		if (auto physicsNode = std::dynamic_pointer_cast<PhysicsHandler>(physics))
		{
			physicsNode->AddInputForce({ 1.f, 0.f });
		}
	}
	//auto trans = _obj->GetWorldTransform();
	//auto pos = trans.GetPosition();
	////Transform2D trans{ { pos.GetX() + 1, pos.GetY() } ,trans.GetRotation(), trans.GetScale() };
	//trans.SetPosition({ pos.GetX() + 1, pos.GetY() });
	//_obj->SetWorldTransform(trans);
}

void MoveUpCommand::Execute(GameObjectDynamic* _obj)
{
	auto comp = _obj->GetChildNode(node::type::PHYSICSHANDLER);
	if (auto physics = comp.lock())
	{
		if (auto physicsNode = std::dynamic_pointer_cast<PhysicsHandler>(physics))
		{
			physicsNode->AddInputForce({0.f, -2.f});
		}
	}

	//auto trans = _obj->GetWorldTransform();
	//auto pos = trans.GetPosition();
	////Transform2D trans{ { pos.GetX() + 1, pos.GetY() } ,trans.GetRotation(), trans.GetScale() };
	//trans.SetPosition({ pos.GetX(), pos.GetY() - 1 });
	//_obj->SetWorldTransform(trans);
}

void MoveDownCommand::Execute(GameObjectDynamic* _obj)
{
	auto comp = _obj->GetChildNode(node::type::PHYSICSHANDLER);
	if (auto physics = comp.lock())
	{
		if (auto physicsNode = std::dynamic_pointer_cast<PhysicsHandler>(physics))
		{
			physicsNode->AddInputForce({ 0.f, 2.f });
		}
	}
	//auto trans = _obj->GetWorldTransform();
	//auto pos = trans.GetPosition();
	////Transform2D trans{ { pos.GetX() + 1, pos.GetY() } ,trans.GetRotation(), trans.GetScale() };
	//trans.SetPosition({ pos.GetX(), pos.GetY() + 1 });
	//_obj->SetWorldTransform(trans);
}

void MoveLeftCommand::Execute(GameObjectDynamic* _obj)
{
	auto comp = _obj->GetChildNode(node::type::PHYSICSHANDLER);
	if (auto physics = comp.lock())
	{
		if (auto physicsNode = std::dynamic_pointer_cast<PhysicsHandler>(physics))
		{
			physicsNode->AddInputForce({ -1.f, 0.f });
		}
	}
	//auto trans = _obj->GetWorldTransform();
	//auto pos = trans.GetPosition();
	////Transform2D trans{ { pos.GetX() + 1, pos.GetY() } ,trans.GetRotation(), trans.GetScale() };
	//trans.SetPosition({ pos.GetX() - 1, pos.GetY() });
	//_obj->SetWorldTransform(trans);
}

void RemoveVoxelCommand::Execute(GameObjectDynamic* _obj)
{

	/*auto pos = GetMousePosition();
	Quad quad{ {pos.x - 25.f, pos.y - 25.f}, {50.f,50.f} };
	auto midPoint = quad.topLeft - quad.getMidPoint();*/
	//m_quadTree->Query(quad, pointsFound);
	/*if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		m_quadTree->InsertVoxels(quad);
	m_quadTree->Draw();
	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
		m_quadTree->RemoveVoxels(quad);*/
}

void InsertVoxelCommand::Execute(GameObjectDynamic* _obj)
{
	//m_quadTree->RemoveVoxels(quad);
}
