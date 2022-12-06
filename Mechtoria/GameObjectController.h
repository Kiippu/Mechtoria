#pragma once
#include "Node.h"
#include "InputHandler.h"
#include "FiniteStateMachine.h"

class InputDevice;
class KeyboardDevice;
class GameObjectDynamic;

class GameObjectController 
    : public DynamicNode
{
public:
	struct GameObjectControllerConfiguration
	{
		network::controllerType type;
		std::shared_ptr<GameObjectDynamic> gameObjectOwner;
		IState* currentState;
		std::shared_ptr<KeyboardDevice> inputDevice;
	};

	GameObjectController(GameObjectControllerConfiguration _params);
	~GameObjectController() {};

	virtual void Update() override;

	void SetGameObjectToControl(std::shared_ptr<GameObjectDynamic> _obj);

	Camera2D& GetControllerCamera();

private:

	GameObjectControllerConfiguration m_configuration;
	std::shared_ptr<InputDevice> m_inputDevice; // controller/keyboard number/ID
	InputHandler m_inputHandler; // consume inputStream
	Camera2D m_camera;
};

