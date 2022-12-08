#include "GamePlayState.h"
#include "SideScrollState.h"
#include "TopDownState.h"
#include "QuadTreeTestState.h"
#include "GameObjectController.h"
#include "InputDevice.h"

std::map < state::type, 
	std::map<node::renderLayer, 
	std::vector<RendableNode*>>> s_rendableNodes = std::map < state::type, std::map<node::renderLayer, std::vector<RendableNode*>>>();

GamePlayState::GamePlayState(FiniteStateMachine* _parent)
	: IState({ node::type::FINITE_STATE_GAMEPLAY, state::type::GAMEPLAY, _parent })
{
}

void GamePlayState::Initialization()
{
	auto stateMachine = std::make_shared<FiniteStateMachine>();
	stateMachine->AddState(std::make_unique<TopDownState>(stateMachine.get(), this));
	stateMachine->AddState(std::make_unique<SideScrollState>(stateMachine.get(), this));
	stateMachine->AddState(std::make_unique<QuadTreeTestState>(stateMachine.get(), this));
	stateMachine->SetState(state::type::SIDE_SCROLL);
	addChildNode(stateMachine);

	GameObjectController::GameObjectControllerConfiguration configCont{
		network::controllerType::LOCAL, nullptr, nullptr,
		std::make_shared<KeyboardDevice>(InputDevice::InputDeviceConfiguration{})
	};

	m_controllers.fill(std::shared_ptr<GameObjectController>());
	m_controllers[0] = std::make_shared<GameObjectController>(configCont);

	Quad bounds = { Vec2<float>{0.f,0.f},Vec2<float>{(float)settings::screenWidth,(float)settings::screenWidth} };
	//m_world[StateType::SIDE_SCROLL] = std::make_unique<QuadTreeV2>(bounds, 4, 5);
	const int capacity = 1;
	const int depth = 5;
	m_worldV2[state::type::SIDE_SCROLL] = std::make_unique<QuadTreeV3>(bounds, capacity, state::type::SIDE_SCROLL, depth);

}

void GamePlayState::Update()
{
	// init all actors in world
	for (auto& actors : m_gameActors)
		InitActors(actors.second);
	// base update
	IState::Update();
	// controller update
	for (auto& controller : m_controllers)
		if(controller)
			controller->Update();

	// world actors update
	for (auto& actor : m_gameActors[node::entity::PAWN_DYNAMIC])
	{
		if (actor)
		{
			actor->Update();
		}
	}

	// update collision in quadtree
	if (const auto FSM = std::dynamic_pointer_cast<FiniteStateMachine>(GetChildNode(node::type::FINITE_STATE_MACHINE).lock()))
	{
		// Remove all stale actors from quadtree
		for (auto& world : m_worldV2)
			world.second->ClearInserts();
		//m_worldV2[FSM->GetActiveState()->GetType()]->ClearInserts();
		// add current actors to collision tree
		for (auto& actorList : m_gameActors)
		{
			if (actorList.first == node::entity::UNDEFINED)
				continue;
			for (auto& actor : actorList.second)
			{
				auto itr = m_worldV2.find(actor->GetOwnerState()->GetType());
				if (itr != m_worldV2.end())
					itr->second->Insert(actor.get());
			}
		}
		// collison update
		m_collision.Update(m_worldV2[FSM->GetActiveState()->GetType()].get());

		auto pos = GetMousePosition();
		auto controller = GetGameObjectControllers()[0];
		auto camera = controller->GetControllerCamera();
		auto worldPos = GetScreenToWorld2D( pos, camera);
		Quad quad{ {worldPos.x - 25.f, worldPos.y - 25.f}, {50.f,50.f} };
		auto midPoint = quad.topLeft - quad.getMidPoint();
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
			m_worldV2[FSM->GetActiveState()->GetType()]->InsertVoxels(quad);
		//m_worldV2[FSM->GetActiveState()->GetType()]->Draw();
		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
			m_worldV2[FSM->GetActiveState()->GetType()]->RemoveVoxels(quad);
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
					//for (auto node : nodeList[node::renderLayer::VOXEL])
					//	node->Draw();
					for (auto node : nodeList[node::renderLayer::GAME_ACTOR_STATIC])
						node->Draw();
					for (auto node : nodeList[node::renderLayer::GAME_ACTOR_PLAYER])
						node->Draw();
					for (auto node : nodeList[node::renderLayer::FOREGROUND])
						node->Draw();
					for (auto node : nodeList[node::renderLayer::DEBUG_NON_GUI])
						node->Draw();
					m_worldV2[state->GetType()]->Draw();
				}
				EndMode2D();
			}
		}
	}
}

void GamePlayState::AddGameActor(std::shared_ptr<GameObjectDynamic> _actor)
{
	const auto entityType = _actor->GetEntityType();
	const auto stateType = _actor->GetOwnerState()->GetType();
	const auto renderLayer = _actor->GetRenderLayer();
	m_gameActors[entityType].push_back(_actor);
	//s_rendableNodes[stateType][renderLayer].push_back(std::dynamic_pointer_cast<RendableNode>(_actor).get());
}

//QuadTreeV3* GamePlayState::GetWorldQuadTree(GameObjectDynamic* obj)
//{
//	obj->GetParent()->GetParent();
//	if (const auto FSM = std::dynamic_pointer_cast<FiniteStateMachine>(GetChildNode(node::type::FINITE_STATE_MACHINE).lock()))
//	{
//
//	}
//
//	return nullptr;
//}

std::array<std::shared_ptr<GameObjectController>, 4>& GamePlayState::GetGameObjectControllers()
{
	return m_controllers;
}

void GamePlayState::InitActors(std::vector<std::shared_ptr<GameObjectDynamic>>& list)
{
	for (auto& actor : list)
	{
		if (actor)
		{
			if (!actor->IsInitialized())
			{
				actor->Initialization();
				actor->SetInitialized();
			}
		}
	}
}

