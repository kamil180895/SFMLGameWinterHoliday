#include "StateManager.h"

StateManager::StateManager(SharedContext * lContext)
	:mContext(lContext)
{
	RegisterState<State_Menu>(StateType::Menu);
	RegisterState<State_Maze>(StateType::Maze);
	RegisterState<State_GameOver>(StateType::GameOver);
	RegisterState<State_Credits>(StateType::Credits);
}

StateManager::~StateManager()
{
	for (int i = 0; i < mStates.size(); ++i)
	{
		mStates[i].second->OnDestroy();
		delete mStates[i].second;
	}
}

void StateManager::Update(const sf::Time & lTime)
{
	if (mStates.empty()) { return; }
	for (int i = mStates.size() - 1; i >= 0; --i)
	{
		if (mStates[i].second->IsTranscendent() == false || i == 0)
		{
			for (; i < mStates.size(); ++i)
			{
				mStates[i].second->Update(lTime);
			}
			break;
		}
	}
}

void StateManager::Draw()
{
	if (mStates.empty()) { return; }
	for (int i = mStates.size() - 1; i >= 0; --i)
	{
		if (mStates[i].second->IsTransparent() == false || i == 0)
		{
			for (; i < mStates.size(); ++i)
			{
				mContext->mWindow->GetRenderWindow()->setView(mStates[i].second->GetView());
				mStates[i].second->Draw();
			}
			break;
		}
	}
}

void StateManager::ProcessRequests()
{
	while (mToRemove.begin() != mToRemove.end())
	{
		RemoveState(*mToRemove.begin());
		mToRemove.erase(mToRemove.begin());
	}
}

SharedContext * StateManager::GetContext()
{
	return mContext;
}

bool StateManager::HasState(const StateType & lState)
{
	for (int i = 0; i < mStates.size(); ++i)
	{
		if (mStates[i].first == lState)
		{
			auto removed = std::find(mToRemove.begin(), mToRemove.end(), lState);
			if(removed == mToRemove.end())
				return true;
			return false;
		}
	}
	return false;
}

void StateManager::SwitchTo(const StateType & lState)
{
	mContext->mEventManager->SetCurrentState(lState);
	for (int i = 0; i < mStates.size(); ++i)
	{
		if (mStates[i].first == lState)
		{
			mStates.back().second->Deactivate();
			StateType type = mStates[i].first;
			BaseState* state = mStates[i].second;
			mStates.erase(mStates.begin() + i);
			mStates.push_back(std::make_pair(type, state));
			mStates.back().second->Activate();
			mContext->mWindow->GetRenderWindow()->setView(state->GetView());
			return;
		}
	}

	if (!mStates.empty()) { mStates.back().second->Deactivate(); }
	CreateState(lState);
	mStates.back().second->Activate();
	mContext->mWindow->GetRenderWindow()->setView(mStates.back().second->GetView());
}

void StateManager::Remove(const StateType & lState)
{
	mToRemove.push_back(lState);
}

void StateManager::CreateState(const StateType & lState)
{
	auto newState = mStateFactory.find(lState);
	if (newState == mStateFactory.end()) { return; }
	BaseState* state = mStateFactory[lState]();
	state->mView = mContext->mWindow->GetDefaultView();
	mStates.emplace_back(lState, state);
	state->OnCreate();
}

void StateManager::RemoveState(const StateType & lState)
{
	for (int i = 0; i < mStates.size(); ++i)
	{
		if (mStates[i].first == lState)
		{
			mStates[i].second->OnDestroy();
			delete mStates[i].second;
			mStates.erase(mStates.begin() + i);
			return;
		}
	}
}
