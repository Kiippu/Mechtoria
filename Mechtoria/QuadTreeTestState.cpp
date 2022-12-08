#include "QuadTreeTestState.h"
#include "GamePlayState.h"
#include "QuadTreeV2.h"
#include "Setting.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <vector>
#include "raylibcpp.h"
#include "Shape.h"
#include "Quad.h"

QuadTreeTestState::QuadTreeTestState(FiniteStateMachine* _parent, GamePlayState* _parentState)
	: IState({ node::type::FINITE_STATE_QUADTREE, state::type::QUADTREE, _parent }), 
	m_gamePlayState(_parentState)
{

	Quad bounds = { Vec2<float>{0.f,0.f},Vec2<float>{(float)settings::screenWidth,(float)settings::screenWidth} };
	int capacity = 1;
	m_quadTree = std::make_unique<QuadTreeV2>(bounds, capacity, 5);

	//srand((unsigned int)time(NULL));
	/*for (size_t i = 0; i < 50; i++)
	{
		Vec2<float> point = { (float)(rand() % settings::screenWidth),(float)(rand() % settings::screenWidth) };
		m_quadTree->Insert(point);
		m_quadPoints.push_back(point);
	}*/
}

void QuadTreeTestState::Update()
{

}

void QuadTreeTestState::Draw()
{

	std::vector <Vec2<float>> pointsFound;

	auto pos = GetMousePosition();
	Quad quad{ {pos.x -25.f, pos.y - 25.f}, {50.f,50.f} };
	auto midPoint = quad.topLeft - quad.getMidPoint();
	//m_quadTree->Query(quad, pointsFound);
	if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		m_quadTree->InsertVoxels(quad);
	m_quadTree->Draw();
	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
		m_quadTree->RemoveVoxels(quad);

	// draw points in rect
	/*for (auto p : pointsFound)
	{
		raycpp::DrawCircle(p, 5.f, GREEN);
	}*/
	DrawRectangleLinesEx({ quad.topLeft.GetX(), quad.topLeft.GetY(), quad.dimentions.GetX(), quad.dimentions.GetY() }, 5, RED);
}
