#pragma once
#include <SFML/Graphics.hpp>
#include "EventManager.h"

class StateManager;
class BaseState
{
	friend class StateManager;
public:
	BaseState(StateManager* lStateManager): mStateManager(lStateManager), mIsTransparent(false), mIsTranscendent(false) {};
	virtual ~BaseState() {};

	virtual void OnCreate() = 0;
	virtual void OnDestroy() = 0;

	virtual void Activate() = 0;
	virtual void Deactivate() = 0;

	virtual void Draw() = 0;
	virtual void Update(const sf::Time& lTime) = 0;

	void SetTransparent(const bool& lTransparent)
	{
		mIsTransparent = lTransparent;
	}
	bool IsTransparent()
	{
		return mIsTransparent;
	}
	void SetTranscendent(const bool& lTranscendent)
	{
		mIsTranscendent = lTranscendent;
	}
	bool IsTranscendent()
	{
		return mIsTranscendent;
	}
	StateManager* GetStateManger()
	{
		return mStateManager;
	}
	sf::View& GetView()
	{
		return mView;
	}
protected:
	StateManager* mStateManager;
	bool mIsTransparent;
	bool mIsTranscendent;
	sf::View mView;
};