#pragma once
#include "BaseState.h"

class State_Credits : public BaseState
{
public:
	State_Credits(StateManager* lStateManager);
	~State_Credits();

	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Draw();
	void Update(const sf::Time& lTime);
private:
	sf::Sprite SCreditsBackground;

	sf::Text exit;

	void GoBackMenu(EventDetails* lDetails);

};