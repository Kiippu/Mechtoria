#pragma once
#include "GameObjectDynamic.h"
#include <memory>

enum class CommandType
{
	// null command
	NO_COMMAND = 0,

	//  actor movement
	MOVE_LEFT = 100,
	MOVE_RIGHT,
	MOVE_UP,
	MOVE_DOWN,

	// state change
	STATE_TOP_DOWN = 200,
	STATE_SIDE_SCROLL,
	STATE_MAIN_MENU

};

class ICommand
{
public: 
	virtual ~ICommand() {};
	virtual void Execute(GameObjectDynamic* _obj) = 0;
};

class MoveLeftCommand : public ICommand
{
public:
	virtual void Execute(GameObjectDynamic* _obj)
	{
		auto trans = _obj->GetWorldTransform();
		auto pos = trans.GetPosition();
		//Transform2D trans{ { pos.GetX() + 1, pos.GetY() } ,trans.GetRotation(), trans.GetScale() };
		trans.SetPosition({ pos.GetX() - 1, pos.GetY() });
		_obj->SetWorldTransform(trans);
	}
};

class MoveRightCommand : public ICommand
{
public:
	virtual void Execute(GameObjectDynamic* _obj)
	{
		auto trans = _obj->GetWorldTransform();
		auto pos = trans.GetPosition();
		//Transform2D trans{ { pos.GetX() + 1, pos.GetY() } ,trans.GetRotation(), trans.GetScale() };
		trans.SetPosition({ pos.GetX() + 1, pos.GetY() });
		_obj->SetWorldTransform(trans);
	}
};

class MoveUpCommand : public ICommand
{
public:
	virtual void Execute(GameObjectDynamic* _obj)
	{
		auto trans = _obj->GetWorldTransform();
		auto pos = trans.GetPosition();
		//Transform2D trans{ { pos.GetX() + 1, pos.GetY() } ,trans.GetRotation(), trans.GetScale() };
		trans.SetPosition({ pos.GetX(), pos.GetY() -1 });
		_obj->SetWorldTransform(trans);
	}
};

class MoveDownCommand : public ICommand
{
public:
	virtual void Execute(GameObjectDynamic* _obj)
	{
		auto trans = _obj->GetWorldTransform();
		auto pos = trans.GetPosition();
		//Transform2D trans{ { pos.GetX() + 1, pos.GetY() } ,trans.GetRotation(), trans.GetScale() };
		trans.SetPosition({ pos.GetX(), pos.GetY() +1 });
		_obj->SetWorldTransform(trans);
	}
};




static std::map<CommandType, std::shared_ptr<ICommand>> CommandFn =
{
	{CommandType::MOVE_LEFT, std::make_shared<MoveLeftCommand>()},
	{CommandType::MOVE_RIGHT, std::make_shared<MoveRightCommand>()},
	{CommandType::MOVE_UP, std::make_shared<MoveUpCommand>()},
	{CommandType::MOVE_DOWN, std::make_shared<MoveDownCommand>()}
};