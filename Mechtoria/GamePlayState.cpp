#include "GamePlayState.h"
#include "SideScrollState.h"
#include "TopDownState.h"
#include "QuadTreeTestState.h"
#include "GameObjectController.h"
#include "InputDevice.h"

std::map < IState::StateType, 
	std::map<node::renderLayer, 
	std::vector<RendableNode*>>> s_rendableNodes = std::map < IState::StateType, std::map<node::renderLayer, std::vector<RendableNode*>>>();

GamePlayState::GamePlayState(FiniteStateMachine* _parent)
	: IState({ node::type::FINITE_STATE_GAMEPLAY, IState::StateType::GAMEPLAY, _parent })
{
}

void GamePlayState::Initialization()
{
	auto stateMachine = std::make_shared<FiniteStateMachine>();
	stateMachine->AddState(std::make_unique<TopDownState>(stateMachine.get(), this));
	stateMachine->AddState(std::make_unique<SideScrollState>(stateMachine.get(), this));
	stateMachine->AddState(std::make_unique<QuadTreeTestState>(stateMachine.get(), this));
	stateMachine->SetState(StateType::QUADTREE);
	addChildNode(stateMachine);

	GameObjectController::GameObjectControllerConfiguration configCont{
		network::controllerType::LOCAL, nullptr, nullptr,
		std::make_shared<KeyboardDevice>(InputDevice::InputDeviceConfiguration{})
	};

	m_controllers.fill(std::shared_ptr<GameObjectController>());
	m_controllers[0] = std::make_shared<GameObjectController>(configCont);

	Quad bounds = { Vec2<float>{0.f,0.f},Vec2<float>{(float)settings::screenWidth,(float)settings::screenWidth} };
	m_world[StateType::SIDE_SCROLL] = std::make_unique<QuadTreeV2>(bounds, 4, 5);
}

void GamePlayState::Update()
{
	// base update
	IState::Update();
	// update current state
	// collison update
	m_collision.Update();
	// controller update
	for (auto& controller : m_controllers)
		if(controller)
			controller->Update();
	// world actors update
	for (auto& actor : m_gameActors[node::entity::PAWN_DYNAMIC])
	{
		if (actor)
		{
			if (!actor->IsInitialized())
			{
				actor->Initialization();
				actor->SetInitialized();
			}
			actor->Update();
		}
	}
}

void GamePlayState::Draw()
{
	if (const auto FSM = std::dynamic_pointer_cast<FiniteStateMachine>(GetChildNode(node::type::FINITE_STATE_MACHINE).lock()))
	{
		const auto state = FSM->GetActiveState();
		if (state)
		{
			state->Draw();
			auto& nodeList = s_rendableNodes[state->GetType()];
			for (auto& v : nodeList)
			{
				std::sort(v.second.begin(), v.second.end(), [](RendableNode* _left, RendableNode* _right) {
					return _left->GetWorldTransform().GetPosition().GetY() < _right->GetWorldTransform().GetPosition().GetY();
				});
			}
			for (auto controller : GetGameObjectControllers())
			{
				if (!controller)
					continue;
				BeginMode2D(controller->GetControllerCamera());
				{
					for (auto node : nodeList[node::renderLayer::BACKGROUND])
						node->Draw();
					for (auto node : nodeList[node::renderLayer::VOXEL])
						node->Draw();
					for (auto node : nodeList[node::renderLayer::GAME_ACTOR_STATIC])
						node->Draw();
					for (auto node : nodeList[node::renderLayer::GAME_ACTOR_PLAYER])
						node->Draw();
					for (auto node : nodeList[node::renderLayer::FOREGROUND])
						node->Draw();
					for (auto node : nodeList[node::renderLayer::DEBUG_NON_GUI])
						node->Draw();
				}
				EndMode2D();
			}
		}
	}
	
}

void GamePlayState::AddGameActor(std::shared_ptr<GameObjectDynamic> _actor, node::entity _type, IState::StateType _state, node::renderLayer _layer)
{
	m_gameActors[_type].push_back(_actor);
	s_rendableNodes[_state][_layer].push_back(std::dynamic_pointer_cast<RendableNode>(_actor).get());
}

std::array<std::shared_ptr<GameObjectController>, 4>& GamePlayState::GetGameObjectControllers()
{
	return m_controllers;
}

