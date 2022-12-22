#include <assert.h>
#include <algorithm>
#include "Engine.h"
#include "raylib.h"
#include "Setting.h"
#include "Node.h"

#include "Shape.h"
#include "GameObjectDynamic.h"
#include "GameObjectController.h"
#include "FiniteStateMachine.h"
#include "MainMenuState.h"
#include "GamePlayState.h"

Engine::Engine(EngineParams _params)
{
	assert(!GetWindowHandle()); // A Window exists but we tried to create another here
	InitWindow(_params.width, _params.height, _params.title.c_str());
	SetTargetFPS(settings::FPS);
	auto stateMachine = std::make_shared<FiniteStateMachine>();
	stateMachine->AddState(std::make_unique<MainMenuState>(stateMachine.get()));
	stateMachine->AddState(std::make_unique<GamePlayState>(stateMachine.get()));
	stateMachine->SetState(state::type::GAMEPLAY);
	m_engineNodes.addChildNode(stateMachine);
}

Engine::~Engine() noexcept
{
	assert(GetWindowHandle()); // A Window doesn't exist but we tried to close it
	CloseWindow();
}

void Engine::Tick()
{
	Update();
	Draw();
}

bool Engine::ExitGameNow() const
{
	return WindowShouldClose();
}

void Engine::Draw()
{
	if (const auto FSM = std::dynamic_pointer_cast<FiniteStateMachine>(m_engineNodes.GetChildNode(node::type::FINITE_STATE_MACHINE).lock()))
	{
		BeginDrawing();
		ClearBackground(WHITE);
		const auto state = FSM->GetActiveState();
		if (!state)
			return;
		if (state->GetType() == state::type::GAMEPLAY)
		{
			GamePlayState* gameState = (GamePlayState*)state;

			gameState->Draw();
		}
		EndDrawing();
	}
	//clear nulls fron render list
	for (auto& stateMap : s_rendableNodes)
	{
		for (auto &renderablesList : stateMap.second)
		{
			auto& drawList = renderablesList.second;
			for (int i = (int)drawList.size()-1; i >= 0; --i)
			{
				if (!drawList[i])
					drawList.erase(drawList.begin() + i);
			}
		}
	}
}

void Engine::Update()
{
	for (auto node : m_engineNodes.GetChildNodes())
	{
		if (auto dynamic = std::dynamic_pointer_cast<UpdateableNode>(node.second))
			dynamic->Update();
	}
}
