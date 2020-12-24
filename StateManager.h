#pragma once
#include "BaseState.h"
#include "SharedContext.h"
#include "State_Menu.h"
#include "State_GameOver.h"
#include "State_Maze.h"
#include "State_Credits.h"
#include <vector>

enum class StateType
{
	Menu = 1, Maze, GameOver, Credits
};

class StateManager
{
public:
	StateManager(SharedContext* lContext);
	~StateManager();

	void Update(const sf::Time& lTime);
	void Draw();

	void ProcessRequests();

	SharedContext* GetContext();
	bool HasState(const StateType& lState);

	void SwitchTo(const StateType& lState);
	void Remove(const StateType& lState);
private:
	void CreateState(const StateType& lState);
	void RemoveState(const StateType& lState);

	template<class T>
	void RegisterState(const StateType& lState)
	{
		mStateFactory[lState] = [this]() -> BaseState*
		{
			return new T(this);
		};
	}

	std::vector<std::pair<StateType, BaseState*>> mStates;
	std::vector<StateType> mToRemove;
	std::unordered_map < StateType, std::function<BaseState*()>> mStateFactory;
	SharedContext* mContext;
};