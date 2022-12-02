#include "SideScrollState.h"

SideScrollState::SideScrollState(FiniteStateMachine* _parent, GamePlayState* _parentState)
	: IState({ node::type::FINITE_STATE_SIDESCROLL, IState::StateType::SIDE_SCROLL}), m_gamePlayState(_parentState)
{

}