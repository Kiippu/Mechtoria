#include "QuadTreeTestState.h"
#include "GamePlayState.h"
#include "QuadTreeV2.h"
#include "Setting.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <vector>
#include "raylibcpp.h"
#include "Shape.h"

QuadTreeTestState::QuadTreeTestState(FiniteStateMachine* _parent, GamePlayState* _parentState)
	: IState({ node::type::FINITE_STATE_QUADTREE, IState::StateType::QUADTREE, _parent }), 
	m_gamePlayState(_parentState)
{

	Quad bounds = { Vec2<float>{0.f,0.f},Vec2<float>{(float)settings::screenWidth,(float)settings::screenWidth} };
	int capacity = 1;
	m_quadTree = std::make_unique<QuadTreeV2>(bounds, capacity);

	srand((unsigned int)time(NULL));
	for (size_t i = 0; i < 200; i++)
	{
		Vec2<float> point = { (float)(rand() % settings::screenWidth),(float)(rand() % settings::screenWidth) };
		m_quadTree->Insert(point);
		m_quadPoints.push_back(point);
	}
}

void QuadTreeTestState::Draw()
{

	std::vector <Vec2<float>> pointsFound;

	auto pos = GetMousePosition();
	Quad quad{ {pos.x -100.f, pos.y - 100.f}, {200.f,200.f} };
	auto midPoint = quad.topLeft - quad.getMidPoint();
	m_quadTree->Query(quad, pointsFound);
	m_quadTree->Draw();

	// draw points in rect
	for (auto p : pointsFound)
	{
		raycpp::DrawCircle(p, 5.f, GREEN);
	}
	DrawRectangleLinesEx({ quad.topLeft.GetX(), quad.topLeft.GetY(), quad.dimentions.GetX(), quad.dimentions.GetY() }, 5, GREEN);
}
