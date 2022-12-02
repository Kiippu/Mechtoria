#include "InputDevice.h"
#include <raylib.h>

void KeyboardDevice::GetInputStream(std::queue<int>& _queue)
{
	int key = GetKeyPressed();
	while ( (KeyboardKey)key != KEY_NULL)
	{
		_queue.push(key);
		m_lastKey = key;
		key = GetKeyPressed();
	}
	if (_queue.empty())
	{
		if (IsKeyDown((KeyboardKey)m_lastKey))
			_queue.push(m_lastKey);
		else
			m_lastKey = KEY_NULL;
	}
}

void GamePadDevice::GetInputStream(std::queue<int>& _queue)
{

}

void NetWorkDevice::GetInputStream(std::queue<int>& _queue)
{

}