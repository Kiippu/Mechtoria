#pragma once
#include <string>
#include "EngineNodePool.h"

class Engine
{
public:
	struct EngineParams
	{
		int width = 0;
		int height = 0;
		std::string title = "";
	};

	Engine(EngineParams _params);
	~Engine() noexcept;
	Engine(const Engine& _engine) = delete;
	Engine& operator=(const Engine& _engine) = delete;

	void Tick();
	bool ExitGameNow() const;

private:
	void Draw();
	void Update();

	EngineNodePool m_engineNodes;

};

