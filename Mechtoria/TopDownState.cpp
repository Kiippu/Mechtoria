#include "TopDownState.h"
#include "raylib.h"
#include "Shape.h"
#include "GameObjectDynamic.h"
#include "GameObjectController.h"
#include "InputDevice.h"
#include "GamePlayState.h"
#include "CollisionVolume.h"
#include "Voxel.h"

TopDownState::TopDownState(FiniteStateMachine* _parent, GamePlayState* _parentState)
	: IState({ node::type::FINITE_STATE_TOPDOWN, IState::StateType::TOP_DOWN, _parent}), m_gamePlayState(_parentState)
{
}

void TopDownState::Initialization()
{
	// test actor
	const float height = (float)GetScreenHeight();
	const float width = (float)GetScreenWidth();
	auto node = std::make_shared<GameObjectDynamic>();
	node->SetWorldTransform(Transform2D{ Vec2<float>{(width / 2.f) - 50.f,(height / 2.f) - 50.f}, 0.f, 1.f });
	Shape::ShapeConfiguration config{
		Shape::ShapeType::rectangle,
		Vec2<float>{100,100},
		BLUE,
		1,
		node
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
	m_gamePlayState->AddGameActor(node, node::entity::PAWN_DYNAMIC, GetType(), node::renderLayer::GAME_ACTOR_PLAYER);

	auto staticNode = std::make_shared<GameObjectDynamic>();
	staticNode->SetWorldTransform(Transform2D{ Vec2<float>{(width / 2) - 200,(height / 2) - 50}, 0.f, 1.f });
	Shape::ShapeConfiguration config_1{
		Shape::ShapeType::rectangle,
		Vec2<float>{50,50},
		BLUE,
		1,
		staticNode
	};
	auto shape_1 = std::make_shared<Shape>(config_1);
	staticNode->addChildNode(shape_1);
	auto collisionVolume_1 = std::make_shared<CollisionVolume>(CollisionVolume::CollisionVolumeConfiguration{ CollisionVolume::VolumeType::rectangle, Vec2<float>{ 50,50 } });
	staticNode->addChildNode(collisionVolume_1);
	m_gamePlayState->AddGameActor(staticNode, node::entity::PAWN_DYNAMIC, GetType(), node::renderLayer::GAME_ACTOR_STATIC);

	//addChildNode(staticNode);
	//int oo{ 0 };
	//m_voxelWorld = std::make_unique<VoxelChonk>(VoxelChonk::VoxelChonkConfiguration{});
	//oo += 43;
}