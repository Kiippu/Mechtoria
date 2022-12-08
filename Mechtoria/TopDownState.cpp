#include "TopDownState.h"
#include "raylib.h"
#include "Shape.h"
#include "GameObjectDynamic.h"
#include "GameObjectController.h"
#include "InputDevice.h"
#include "GamePlayState.h"
#include "CollisionVolume.h"

TopDownState::TopDownState(FiniteStateMachine* _parent, GamePlayState* _parentState)
	: IState({ node::type::FINITE_STATE_TOPDOWN, state::type::TOP_DOWN, _parent}), m_gamePlayState(_parentState)
{
}

void TopDownState::Initialization()
{
	// test actor
	const float height = (float)GetScreenHeight();
	const float width = (float)GetScreenWidth();
	auto node = std::make_shared<GameObjectDynamic>(this, node::renderLayer::GAME_ACTOR_PLAYER, node::entity::PAWN_DYNAMIC);
	//auto node = std::make_shared<GameObjectDynamic>(state::type::TOP_DOWN, this, node::entity::PAWN_DYNAMIC);
	node->SetWorldTransform(Transform2D{ Vec2<float>{(width / 2.f) - 50.f,(height / 2.f) - 50.f}, 0.f, 1.f });
	Shape::ShapeConfiguration config{
		Shape::ShapeType::rectangle,
		Vec2<float>{100,100},
		BLUE,
		1,
		node,
		state::type::TOP_DOWN
	};
	auto shape = std::make_shared<Shape>(config);
	node->addChildNode(shape);
	GameObjectController::GameObjectControllerConfiguration configCont{
		network::controllerType::LOCAL, node, nullptr,
		std::make_shared<KeyboardDevice>(InputDevice::InputDeviceConfiguration{})
	};
	auto collisionVolume = std::make_shared<CollisionVolume>(CollisionVolume::CollisionVolumeConfiguration{});
	node->addChildNode(collisionVolume);
	m_gamePlayState->GetGameObjectControllers()[0]->SetGameObjectToControl(node);
	m_gamePlayState->AddGameActor(node);

	auto staticNode = std::make_shared<GameObjectDynamic>(this, node::renderLayer::GAME_ACTOR_STATIC, node::entity::PAWN_STATIC);
	staticNode->SetWorldTransform(Transform2D{ Vec2<float>{(width / 2) - 200,(height / 2) - 50}, 0.f, 1.f });
	Shape::ShapeConfiguration config_1{
		Shape::ShapeType::rectangle,
		Vec2<float>{50,50},
		BLUE,
		1,
		staticNode,
		state::type::TOP_DOWN
	};
	auto shape_1 = std::make_shared<Shape>(config_1);
	staticNode->addChildNode(shape_1);
	auto collisionVolume_1 = std::make_shared<CollisionVolume>(CollisionVolume::CollisionVolumeConfiguration{ CollisionVolume::VolumeType::rectangle, Vec2<float>{ 50,50 } });
	staticNode->addChildNode(collisionVolume_1);
	m_gamePlayState->AddGameActor(staticNode);

}