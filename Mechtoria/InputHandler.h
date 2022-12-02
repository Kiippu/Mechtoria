#pragma once
#include <map>
#include "Node.h"
#include "Command.h"
#include "Setting.h"

class InputDevice;
class InputHandler
{
public:
	InputHandler(std::weak_ptr<InputDevice> _inputDevice);

	ICommand* HandleInput();

	std::map<KeyboardKey, CommandType> m_keyboardKeyComamnds;
	std::map<MouseButton, CommandType> m_mouseButtonComamnds;
	std::map<GamepadButton, CommandType> m_GamePadButtonComamnds;
	std::map<GamepadAxis, CommandType> m_GamePadAxisComamnds;

	std::weak_ptr<InputDevice> m_inputDevice;
private:
	ICommand* GetCommand(CommandType _cmdType);
};

