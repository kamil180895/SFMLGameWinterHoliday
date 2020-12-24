#include "EventManager.h"

void EventManager::SetupBindingsManualy()
{
	Binding* newBinding = new Binding;
	newBinding->BindEvent(EventType::Closed, KeyCode());
	AddBinding("Close", newBinding);
	newBinding = new Binding;
	newBinding->BindEvent(EventType::KeyDown, KeyCode(sf::Keyboard::F5));
	AddBinding("ToggleFullscreen", newBinding);
	newBinding = new Binding;
	newBinding->BindEvent(EventType::KeyDown, KeyCode(sf::Keyboard::W));
	AddBinding("W", newBinding);
	newBinding = new Binding;
	newBinding->BindEvent(EventType::KeyDown, KeyCode(sf::Keyboard::S));
	AddBinding("S", newBinding);
	newBinding = new Binding;
	newBinding->BindEvent(EventType::KeyDown, KeyCode(sf::Keyboard::A));
	AddBinding("A", newBinding);
	newBinding = new Binding;
	newBinding->BindEvent(EventType::KeyDown, KeyCode(sf::Keyboard::D));
	AddBinding("D", newBinding);
	newBinding = new Binding;
	newBinding->BindEvent(EventType::Keyboard, KeyCode(sf::Keyboard::A));
	AddBinding("RealTimeA", newBinding);
	newBinding = new Binding;
	newBinding->BindEvent(EventType::Keyboard, KeyCode(sf::Keyboard::D));
	AddBinding("RealTimeD", newBinding);
	newBinding = new Binding;
	newBinding->BindEvent(EventType::Keyboard, KeyCode(sf::Keyboard::W));
	AddBinding("RealTimeW", newBinding);
	newBinding = new Binding;
	newBinding->BindEvent(EventType::Keyboard, KeyCode(sf::Keyboard::S));
	AddBinding("RealTimeS", newBinding);
	newBinding = new Binding;
	newBinding->BindEvent(EventType::KeyDown, KeyCode(sf::Keyboard::Enter));
	AddBinding("Enter", newBinding);
	newBinding = new Binding;
	newBinding->BindEvent(EventType::KeyDown, KeyCode(sf::Keyboard::H));
	AddBinding("H", newBinding);
}

EventManager::EventManager()
	: mHasFocus(true)
{
	SetupBindingsManualy();
}

EventManager::~EventManager()
{
	auto itr = mBinding.begin();
	while (itr != mBinding.end())
	{
		delete itr->second;
		++itr;
	}
}

bool EventManager::AddBinding(const std::string & lName, Binding * lBinding)
{
	if (mBinding.find(lName) != mBinding.end())
		return false;

	mBinding[lName] = lBinding;
	return true;
}

bool EventManager::RemoveBinding(const std::string & lName)
{
	if (mBinding.find(lName) == mBinding.end())
		return false;

	delete mBinding.find(lName)->second;
	mBinding.erase(lName);

	return true;
}

bool EventManager::RemoveCallback(const StateType& lType, const std::string & lName)
{
	auto itr = mCallback.find(lType);
	if (itr == mCallback.end())
		return false;
	auto itr2 = itr->second.find(lName);
	if (itr2 == itr->second.end()) { return false; }
	itr->second.erase(lName);
	return true;
}

void EventManager::Update()
{
	if (!mHasFocus)
		return;

	auto itr_b = mBinding.begin();
	while (itr_b != mBinding.end())
	{
		auto events = itr_b->second->mEvents;
		auto itr_e = events.begin();
		while (itr_e != events.end())
		{
			if (itr_e->first == EventType::Keyboard)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(itr_e->second.mCode)))
				{
					if (itr_b->second->mDetails.m_keyCode != -1) {
						itr_b->second->mDetails.m_keyCode = itr_e->second.mCode;
					}

					++(itr_b->second->count);
				}
			}
			else if (itr_e->first == EventType::Mouse)
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button(itr_e->second.mCode)))
				{
					if (itr_b->second->mDetails.m_keyCode != -1) {
						itr_b->second->mDetails.m_keyCode = itr_e->second.mCode;
					}

					++(itr_b->second->count);
				}
			}
			++itr_e;
		}

		if (itr_b->second->mEvents.size() == itr_b->second->count)
		{
			auto stateCallbacks = mCallback.find(mCurrentState);
			auto otherCallbacks = mCallback.find(StateType(0));

			if (stateCallbacks != mCallback.end())
			{
				auto call = stateCallbacks->second.find(itr_b->first);
				if (call != stateCallbacks->second.end())
					call->second(&itr_b->second->mDetails);
			}

			if (otherCallbacks != mCallback.end())
			{
				auto call = otherCallbacks->second.find(itr_b->first);
				if (call != otherCallbacks->second.end())
					call->second(&itr_b->second->mDetails);
			}
		}
		itr_b->second->count = 0;
		itr_b->second->mDetails.Clear();
		++itr_b;
	}
}

void EventManager::HandleEvent(const sf::Event & lEvent)
{
	auto itr_b = mBinding.begin();
	while (itr_b != mBinding.end())
	{
		auto events = itr_b->second->mEvents;
		auto itr_e = events.begin();
		while (itr_e != events.end())
		{
			EventType type = (EventType)lEvent.type;
			if (type != itr_e->first)
			{
				++itr_e;
				continue;
			}
			if (type == EventType::KeyDown || type == EventType::KeyUp)
			{
				if (itr_e->second.mCode == lEvent.key.code)
				{
					if (itr_b->second->mDetails.m_keyCode != -1) {
						itr_b->second->mDetails.m_keyCode = itr_e->second.mCode;
					}

					++(itr_b->second->count);
					break;
				}
					
			}
			else if(type == EventType::MButtonDown || type == EventType::MButtonUp)
			{
				if (itr_e->second.mCode == lEvent.mouseButton.button)
				{
					itr_b->second->mDetails.m_mouse.x = lEvent.mouseButton.x;
					itr_b->second->mDetails.m_mouse.y = lEvent.mouseButton.y;
					if (itr_b->second->mDetails.m_keyCode != -1) {
						itr_b->second->mDetails.m_keyCode = itr_e->second.mCode;
					}
					++(itr_b->second->count);
					break;
				}
			}
			else
			{
				if (type == EventType::MouseWheel) {
					itr_b->second->mDetails.m_mouseWheelDelta = lEvent.mouseWheel.delta;
				}
				else if (type == EventType::WindowResized) {
					itr_b->second->mDetails.m_size.x = lEvent.size.width;
					itr_b->second->mDetails.m_size.y = lEvent.size.height;
				}
				else if (type == EventType::TextEntered) {
					itr_b->second->mDetails.m_textEntered = lEvent.text.unicode;
				}
				++(itr_b->second->count);
				break;
			}
				
			++itr_e;
		}
		++itr_b;
	}
}

void EventManager::SetFocus(bool lFocus)
{
	mHasFocus = lFocus;
}

void EventManager::SetCurrentState(const StateType & lType)
{
	mCurrentState = lType;
}
