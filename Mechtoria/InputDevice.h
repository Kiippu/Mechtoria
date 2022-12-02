#pragma once
#include <queue>

class InputDevice
{
public:
	enum class InputDeviceType
	{
		KEYBOARD,
		GAMEPAD,
		NETWORK
	};
	struct InputDeviceConfiguration
	{
	};
	InputDevice(InputDeviceConfiguration _params, InputDeviceType _type)
		: m_configuration(_params), m_type(_type), m_lastKey(0)
	{}
	virtual ~InputDevice() {};
	virtual void GetInputStream(std::queue<int>& _queue) = 0;
	InputDeviceType GetType() const { return m_type; }
protected:
	InputDeviceConfiguration m_configuration;
	InputDeviceType m_type;
	int m_lastKey;
};


class KeyboardDevice : public InputDevice
{
public:
	KeyboardDevice(InputDeviceConfiguration _params)
		: InputDevice(_params, InputDeviceType::KEYBOARD)
	{}
	virtual ~KeyboardDevice() {}
	virtual void GetInputStream(std::queue<int>& _queue) override;
private:

};


class GamePadDevice : public InputDevice
{
public:
	GamePadDevice(InputDeviceConfiguration _params)
		: InputDevice(_params, InputDeviceType::GAMEPAD)
	{}
	virtual ~GamePadDevice() {}
	virtual void GetInputStream(std::queue<int>& _queue) override;
private:

};


class NetWorkDevice : public InputDevice
{
public:
	NetWorkDevice(InputDeviceConfiguration _params)
		: InputDevice(_params, InputDeviceType::NETWORK)
	{}
	virtual ~NetWorkDevice() {}
	virtual void GetInputStream(std::queue<int>& _queue) override;
private:

};