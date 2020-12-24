#pragma once
#include "BaseState.h"
#include "Maze.h"
#include "Player.h"

class State_Maze : public BaseState
{
public:
	State_Maze(StateManager* lStateManager);
	~State_Maze();

	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Draw();
	void Update(const sf::Time& lTime);
private:
	void GoUp(EventDetails* lDetails);
	void GoDown(EventDetails* lDetails);
	void GoLeft(EventDetails* lDetails);
	void GoRight(EventDetails* lDetails);
	void ToggleDebug(EventDetails* lDetails);

	float mZoom;

	Maze mMaze;
	Player mPlayer;
	sf::Sprite mFilter;
	bool mDebugMode;
};