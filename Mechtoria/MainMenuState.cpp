#include "MainMenuState.h"


MainMenuState::MainMenuState(FiniteStateMachine* _parent)
	: IState({ node::type::FINITE_STATE_MAINMENU, state::type::MAIN_MENU, _parent })
{
}