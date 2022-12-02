#pragma once
#include "Node.h"
#include <vector>

class FiniteStateMachine;
class IState : public Visual2DDynamicNode
{
public:
	enum class StateType
	{
		MAIN_MENU,
		TOP_DOWN,
		SIDE_SCROLL,
		GAMEPLAY,
		QUADTREE
	};
	struct IStateConfiguration
	{
		node::type nodeType;
		StateType type;
		FiniteStateMachine* stateMachine = nullptr;
		IState* parentState = nullptr;

	};
	IState(IStateConfiguration _params);
	virtual ~IState() = default;
	/*IState(const IState& _node) = default;
	IState& operator=(const IState& _otherNode) = default;
	IState(IState&& _node) = default;
	IState& operator=(IState&& _otherNode) = default;*/

	StateType GetType() const { return m_type; }
	FiniteStateMachine* GetFiniteStateMachineParent() const { return m_parent; };
	const IStateConfiguration& GetConfiguration() const { return m_configuration; }

private:
	StateType m_type;
	FiniteStateMachine* m_parent;
	IStateConfiguration m_configuration;
};

class FiniteStateMachine : public Visual2DDynamicNode
{
public:
	FiniteStateMachine();

	virtual void Initialization() override {};
	virtual void Deinitialization() override {};

	virtual void Update() override;

	virtual void Draw() override;

	void SetState(IState::StateType _type) { m_stateType = _type; };
	IState* GetActiveState() const { return m_currentState; };

	void AddState(std::unique_ptr<IState> _state);


private:
	IState* m_currentState;
	IState::StateType m_stateType;
	std::vector<std::unique_ptr<IState>> m_childrenStates;
};