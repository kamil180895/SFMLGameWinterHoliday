#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <functional>

enum class EventType {
	KeyDown = sf::Event::KeyPressed,
	KeyUp = sf::Event::KeyReleased,
	MButtonDown = sf::Event::MouseButtonPressed,
	MButtonUp = sf::Event::MouseButtonReleased,
	MouseWheel = sf::Event::MouseWheelMoved,
	WindowResized = sf::Event::Resized,
	GainedFocus = sf::Event::GainedFocus,
	LostFocus = sf::Event::LostFocus,
	MouseEntered = sf::Event::MouseEntered,
	MouseLeft = sf::Event::MouseLeft,
	Closed = sf::Event::Closed,
	TextEntered = sf::Event::TextEntered,
	Keyboard = sf::Event::Count + 1, Mouse, Joystick
};

struct KeyCode
{
	KeyCode() : mCode(-1) {}
	KeyCode(int lCode) : mCode(lCode){}
	int mCode;
};

struct EventDetails
{
	EventDetails()
	{
		Clear();
	}

	sf::Vector2i m_size;
	sf::Uint32 m_textEntered;
	sf::Vector2i m_mouse;
	int m_mouseWheelDelta;
	int m_keyCode; // Single key code.

	void Clear() {
		m_size = sf::Vector2i(0, 0);
		m_textEntered = 0;
		m_mouse = sf::Vector2i(0, 0);
		m_mouseWheelDelta = 0;
		m_keyCode = -1;
	}
};

struct Binding
{
	Binding() : count(0){}
	void BindEvent(EventType lType, KeyCode lCode = KeyCode())
	{
		mEvents.push_back(std::make_pair(lType, lCode));
	}

	EventDetails mDetails;
	std::vector<std::pair<EventType, KeyCode>> mEvents;
	int count;
};

enum class StateType;
class EventManager
{
public:
	EventManager();
	~EventManager();

	bool AddBinding(const std::string &lName, Binding* lBinding);
	bool RemoveBinding(const std::string &lName);

	template<class T>
	bool AddCallback(const StateType& lType, const std::string &lName, void(T::*lFunc)(EventDetails*), T* lInstance)
	{
		auto itr = mCallback.emplace(lType, std::unordered_map<std::string, std::function<void(EventDetails*)>>()).first;
		auto temp = std::bind(lFunc, lInstance, std::placeholders::_1);
		return itr->second.emplace(lName, temp).second;
	}
	bool RemoveCallback(const StateType& lType, const std::string &lName);

	void Update();
	void HandleEvent(const sf::Event& lEvent);

	void SetFocus(bool lFocus);
	void SetCurrentState(const StateType& lType);
private:
	void SetupBindingsManualy();

	std::unordered_map<std::string, Binding*> mBinding;
	std::unordered_map<StateType, std::unordered_map<std::string, std::function<void(EventDetails*)>>> mCallback;
	bool mHasFocus;
	StateType mCurrentState;
};