#include "Game.h"
#include "raylib.h"
#include "Setting.h"

Game::Game(int width, int height, std::string title)
{
	assert(!GetWindowHandle()); // A Window exists but we tried to create another here
	InitWindow(width, height, title.c_str());
}

Game::~Game() noexcept
{
	assert(GetWindowHandle()); // A Window doesn't exist but we tried to close it
	SetTargetFPS(settings::FPS);
	CloseWindow();
}

void Game::Tick()
{
	BeginDrawing();
		Update();
		Draw();
	EndDrawing();
}

bool Game::ExitGameNow() const
{
	return WindowShouldClose();
}

void Game::Draw()
{
	ClearBackground(BLACK);
}

void Game::Update()
{
}
