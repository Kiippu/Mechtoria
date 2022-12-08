#include "SideScrollState.h"
#include "CollisionHandler.h"
#include "GameObjectController.h"
#include "CollisionVolume.h"
#include "InputDevice.h"
#include "GamePlayState.h"


SideScrollState::SideScrollState(FiniteStateMachine* _parent, GamePlayState* _parentState)
	: IState({ node::type::FINITE_STATE_SIDESCROLL, state::type::SIDE_SCROLL}), m_gamePlayState(_parentState)
{}

void SideScrollState::Initialization()
{
	// test actor
	const float height = (float)GetScreenHeight();
	const float width = (float)GetScreenWidth();


	for (size_t i = 1; i < 11; i++)
	{
		auto w = (width / 11.f) * i;
		auto h = (height / 11.f) * i;
		auto staticNode = std::make_shared<GameObjectDynamic>(this, node::renderLayer::GAME_ACTOR_STATIC, node::entity::PAWN_STATIC);
		staticNode->SetWorldTransform(Transform2D{ Vec2<float>{ w,h}, 0.f, 1.f });
		CollisionVolume::CollisionVolumeConfiguration configStatic
		{
			CollisionVolume::VolumeType::rectangle,
			{ 50.f,50.f },
			RED,
			DARKBLUE,
			5,
			state::type::SIDE_SCROLL
		};
		auto collisionVol = std::make_shared<CollisionVolume>(configStatic);
		staticNode->addChildNode(collisionVol);
		m_gamePlayState->AddGameActor(staticNode);
	}

	auto node = std::make_shared<GameObjectDynamic>( this, node::renderLayer::GAME_ACTOR_PLAYER, node::entity::PAWN_DYNAMIC);
	node->SetWorldTransform(Transform2D{ Vec2<float>{(width / 2.f) - 50.f,(height / 2.f) - 50.f}, 0.f, 1.f });

	GameObjectController::GameObjectControllerConfiguration configCont{
		network::controllerType::LOCAL, node, nullptr,
		std::make_shared<KeyboardDevice>(InputDevice::InputDeviceConfiguration{})
	};
	CollisionVolume::CollisionVolumeConfiguration configVol
	{
		CollisionVolume::VolumeType::rectangle,
		{ 100.f,100.f },
		RED,
		BLACK,
		5,
		state::type::SIDE_SCROLL
	};
	auto collisionVolume = std::make_shared<CollisionVolume>(configVol);
	node->addChildNode(collisionVolume);
	m_gamePlayState->GetGameObjectControllers()[0]->SetGameObjectToControl(node);
	m_gamePlayState->AddGameActor(node);

	Quad bounds = { Vec2<float>{0.f,0.f},Vec2<float>{(float)settings::screenWidth,(float)settings::screenWidth} };
	int capacity = 1;
	/*m_quadTree = std::make_unique<QuadTreeV3>(bounds, capacity, 5);


	m_quadTree->Insert(node.get());

	m_collision = std::make_unique<CollisionHandler>();*/
}



void SideScrollState::Update()
{

}

void SideScrollState::Draw()
{
	std::vector<GameObjectDynamic*> pointsFound;

	auto pos = GetMousePosition();
	Quad quad{ {pos.x - 25.f, pos.y - 25.f}, {50.f,50.f} };
	auto midPoint = quad.topLeft - quad.getMidPoint();
	//m_quadTree->Query(quad, pointsFound);
	/*if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		m_quadTree->InsertVoxels(quad);
	m_quadTree->Draw();
	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
		m_quadTree->RemoveVoxels(quad);*/

	//m_collision->Update(m_quadTree.get());

	// draw points in rect
	/*for (auto p : pointsFound)
	{
		raycpp::DrawCircle(p, 5.f, GREEN);
	}*/
	DrawRectangleLinesEx({ quad.topLeft.GetX(), quad.topLeft.GetY(), quad.dimentions.GetX(), quad.dimentions.GetY() }, 5, RED);
}
