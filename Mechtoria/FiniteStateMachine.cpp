#include "FiniteStateMachine.h"

FiniteStateMachine::FiniteStateMachine()
	: Visual2DDynamicNode(node::type::FINITE_STATE_MACHINE, node::renderLayer::SKIP_NODE, state::type::SKIP), m_currentState(nullptr), m_stateType(state::type::GAMEPLAY)
{
}

void FiniteStateMachine::Update()
{

	auto& nodes = GetChildNodes();
	for (auto& node : nodes)
	{
		if (auto update = std::dynamic_pointer_cast<UpdateableNode>(node.second))
		{
			update->Update();
		}
	}

	for (auto& state : m_childrenStates)
	{
		// update all states
		if (!state->IsInitialized())
		{
			state->Initialization();
			state->SetInitialized();
		}
		state->Update();
		if (state->GetType() == m_stateType)
			m_currentState = state.get();
	}
}

void FiniteStateMachine::Draw()
{
	if (m_currentState)
		m_currentState->Draw();
}

void FiniteStateMachine::AddState(std::unique_ptr<IState> _state)
{
	m_childrenStates.push_back(std::move(_state));
}

IState::IState(IStateConfiguration _params)
	:  Visual2DDynamicNode(_params.nodeType, node::renderLayer::SKIP_NODE, _params.type), m_type(_params.type), m_parent(_params.stateMachine), m_configuration(_params)
{
}