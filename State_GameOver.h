#pragma once
#include "BaseState.h"

class State_GameOver : public BaseState
{
public:
	State_GameOver(StateManager* lStateManager);
	~State_GameOver();

	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Draw();
	void Update(const sf::Time& lTime);
private:
	void ChangeOptionLeft(EventDetails* lDetails);
	void ChangeOptionRight(EventDetails* lDetails);
	void Enter(EventDetails* lDetails);

	sf::Text mCongrats;
	sf::Text mNewGame;
	sf::Text mExit;

	int mOption;
};