#pragma once
#include "BaseState.h"
#include "EventManager.h"
#include <string>

class State_Menu : public BaseState
{
public:
	State_Menu(StateManager* lStateManager);
	~State_Menu();

	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Draw();
	void Update(const sf::Time& lTime);
private:
	unsigned int mCurrentOption;

	sf::Sprite SMainMenuBackground;

	sf::Text play;
	sf::Text credits;
	sf::Text exit;

	void ChangeOptionUp(EventDetails *lDetails);
	void ChangeOptionDown(EventDetails *lDetails);
	
	void ChooseOptionEnter(EventDetails* lDetails);
};