#include "InputHandler.h"
#include "CommandBindings.h"
#include "InputDevice.h"

InputHandler::InputHandler(std::weak_ptr<InputDevice> _inputDevice)
	: m_inputDevice(_inputDevice)
{
	m_keyboardKeyComamnds = s_keyboardKeyCommands;
	m_GamePadButtonComamnds = s_gamepadButtonCommands;
	m_GamePadAxisComamnds = s_gameAxisCommands;
	m_mouseButtonComamnds = s_mouseButtonCommands;

	m_keyboardKeyComamnds[KEY_LEFT] = CommandType::MOVE_LEFT;
	m_keyboardKeyComamnds[KEY_RIGHT] = CommandType::MOVE_RIGHT;
	m_keyboardKeyComamnds[KEY_UP] = CommandType::MOVE_UP;
	m_keyboardKeyComamnds[KEY_DOWN] = CommandType::MOVE_DOWN;
};

ICommand* InputHandler::HandleInput()
{
	std::queue<int> inputStream;
	if (auto input = m_inputDevice.lock())
	{
		input->GetInputStream(inputStream);
		while (!inputStream.empty())
		{
			auto inputId = inputStream.front();
			switch (input->GetType())
			{
			case InputDevice::InputDeviceType::GAMEPAD:
			{
				auto itr = m_GamePadButtonComamnds.find((GamepadButton)inputId);
				if (itr != m_GamePadButtonComamnds.end())
				{
					return GetCommand(itr->second);
				}
			}
				break;
			case InputDevice::InputDeviceType::KEYBOARD:
			{
				auto itr = m_keyboardKeyComamnds.find((KeyboardKey)inputId);
				if (itr != m_keyboardKeyComamnds.end())
				{
					return GetCommand(itr->second);
				}
			}
				break;
			case InputDevice::InputDeviceType::NETWORK:
				break;
			default:
				return nullptr;
				break;
			}
		}
	}
    return nullptr;
}

ICommand* InputHandler::GetCommand(CommandType _cmdType)
{
	const auto command = CommandFn.find(_cmdType);
	if (command == CommandFn.end())
		return nullptr;
	return (command->second) ? command->second.get() : nullptr;
}
