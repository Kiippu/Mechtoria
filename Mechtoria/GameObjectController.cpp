#include "GameObjectController.h"
#include"InputDevice.h"
#include <raylib.h>


GameObjectController::GameObjectController(GameObjectControllerConfiguration _params)
	: DynamicNode(node::type::CONTROLLER), 
	m_configuration(_params),
	m_inputDevice(std::make_shared<KeyboardDevice>(InputDevice::InputDeviceConfiguration{})),
	m_inputHandler(m_inputDevice),
	m_camera({0})
{
	m_camera.offset = { (GetRenderWidth() / 2.0f)-50, (GetRenderHeight() / 2.0f) -50 };
	m_camera.rotation = 0.0f;
	m_camera.zoom = 1.0f;
}

void GameObjectController::Update()
{
	if (!m_configuration.gameObjectOwner)
		return;
	if (ICommand* command = m_inputHandler.HandleInput())
	{
		command->Execute(m_configuration.gameObjectOwner.get());
	}

	m_camera.target = m_configuration.gameObjectOwner->GetWorldTransform().GetPosition_RayLib();
}


void GameObjectController::SetGameObjectToControl(std::shared_ptr<GameObjectDynamic> _obj)
{
	m_configuration.gameObjectOwner = _obj;
}

Camera2D& GameObjectController::GetControllerCamera()
{
	return m_camera;
}
