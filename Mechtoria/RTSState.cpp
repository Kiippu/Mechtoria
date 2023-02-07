#include "RTSState.h"
#include "AIActor.h"
#include "GameObjectController.h"


RTSState::RTSState(FiniteStateMachine* _parent, GamePlayState* _parentState)
	: IState({ node::type::FINITE_STATE_RTS, state::type::RTS }), m_gamePlayState(_parentState)
{}

void RTSState::Initialization()
{

	auto node = std::make_shared<AIActor>(this, node::renderLayer::GAME_ACTOR_PLAYER, node::entity::PAWN_DYNAMIC);
	node->SetWorldTransform(Transform2D{ Vec2<float>{50.f,50.f}, 0.f, 1.f });
	node->SetName("PlayerGameObj");

	m_gamePlayState->AddGameActor(std::dynamic_pointer_cast<GameObjectDynamic>(node));

}

void RTSState::Update()
{
}

void RTSState::Draw()
{
}
