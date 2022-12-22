#include "SideScrollState.h"
#include "CollisionHandler.h"
#include "GameObjectController.h"
#include "CollisionVolume.h"
#include "InputDevice.h"
#include "GamePlayState.h"
#include "PlayerActor.h"

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */


SideScrollState::SideScrollState(FiniteStateMachine* _parent, GamePlayState* _parentState)
	: IState({ node::type::FINITE_STATE_SIDESCROLL, state::type::SIDE_SCROLL}), m_gamePlayState(_parentState)
{}

void SideScrollState::Initialization()
{
	// test actor
	const float height = (float)GetScreenHeight();
	const float width = (float)GetScreenWidth();

	//srand((unsigned int)time(NULL));
	//const size_t randomActorCount = 31;
	//for (size_t i = 1; i < randomActorCount; i++)
	//{
	//	auto w = (float)(rand() % settings::screenWidth);//(width / 2.f) - 150.f;
	//	auto h = (float)(rand() % settings::screenWidth);//(height / 2.f) + 50.f;
	//	auto staticNode = std::make_shared<GameObjectDynamic>(this, node::renderLayer::GAME_ACTOR_STATIC, node::entity::PAWN_STATIC);
	//	staticNode->SetWorldTransform(Transform2D{ Vec2<float>{ w,h}, 0.f, 1.f });
	//	CollisionVolume::CollisionVolumeConfiguration configStatic
	//	{
	//		CollisionVolume::VolumeType::rectangle,
	//		{ 50.f,50.f },
	//		RED,
	//		DARKBLUE,
	//		5,
	//		state::type::SIDE_SCROLL
	//	};
	//	auto collisionVol = std::make_shared<CollisionVolume>(configStatic);
	//	staticNode->addChildNode(collisionVol);
	//	m_gamePlayState->AddGameActor(staticNode);
	//}

	auto node = std::make_shared<PlayerActor>( this, node::renderLayer::GAME_ACTOR_PLAYER, node::entity::PAWN_DYNAMIC);
	node->SetWorldTransform(Transform2D{ Vec2<float>{-50.f,-150.f}, 0.f, 1.f });
	node->SetName("PlayerGameObj");

	m_gamePlayState->GetGameObjectControllers()[0]->SetGameObjectToControl(std::dynamic_pointer_cast<GameObjectDynamic>(node));
	m_gamePlayState->AddGameActor(std::dynamic_pointer_cast<GameObjectDynamic>(node));

	//Quad bounds = { Vec2<float>{0.f,0.f},Vec2<float>{(float)settings::screenWidth,(float)settings::screenWidth} };
	//int capacity = 1;
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
	Quad quad{ {pos.x - 5.f, pos.y - 5.f}, {10.f,10.f} };
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
	DrawRectangleLinesEx({ quad.topLeft.GetX(), quad.topLeft.GetY(), quad.dimentions.GetX(), quad.dimentions.GetY() }, 5, ORANGE);
}
