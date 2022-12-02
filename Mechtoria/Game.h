#pragma once

#include <string>
#include <assert.h>

class Game
{
public:
	Game(int width, int height, std::string);
	~Game() noexcept;
	Game(const Game& game) = delete;
	Game& operator=(const Game& other) = delete;

	void Tick();
	bool ExitGameNow() const;

private:
	void Draw();
	void Update();
};
 
