#include "State_GameOver.h"
#include "StateManager.h"

State_GameOver::State_GameOver(StateManager * lStateManager)
	:BaseState(lStateManager), mOption(0)
{
}

State_GameOver::~State_GameOver()
{
}

void State_GameOver::OnCreate()
{
	mStateManager->GetContext()->mEventManager->AddCallback(StateType::GameOver, "A", &State_GameOver::ChangeOptionLeft, this);
	mStateManager->GetContext()->mEventManager->AddCallback(StateType::GameOver, "D", &State_GameOver::ChangeOptionRight, this);
	mStateManager->GetContext()->mEventManager->AddCallback(StateType::GameOver, "Enter", &State_GameOver::Enter, this);
	mStateManager->GetContext()->mFontManager->RequireResource("MainFont");

	sf::Font* MainFont = mStateManager->GetContext()->mFontManager->GetResource("MainFont");
	mCongrats.setFont(*MainFont);
	mCongrats.setString("Congratulation! You've succesfully escaped maze.");
	mCongrats.setCharacterSize(30);
	mCongrats.setPosition(sf::Vector2f(120.0, 170.0));

	mNewGame.setFont(*MainFont);
	mNewGame.setString(">Generate new level<");
	mNewGame.setCharacterSize(30);
	mNewGame.setPosition(sf::Vector2f(100.0, 220.0));

	mExit.setFont(*MainFont);
	mExit.setString(">Exit<");
	mExit.setCharacterSize(30);
	mExit.setPosition(sf::Vector2f(500.0, 220.0));

}

void State_GameOver::OnDestroy()
{
	mStateManager->GetContext()->mEventManager->RemoveCallback(StateType::GameOver, "A");
	mStateManager->GetContext()->mEventManager->RemoveCallback(StateType::GameOver, "D");
	mStateManager->GetContext()->mEventManager->RemoveCallback(StateType::GameOver, "Enter");
	mStateManager->GetContext()->mFontManager->ReleaseResource("MainFont");
}

void State_GameOver::Activate()
{
	mStateManager->Remove(StateType::Maze);
}

void State_GameOver::Deactivate()
{
}

void State_GameOver::Draw()
{
	mStateManager->GetContext()->mWindow->Draw(mCongrats);
	mStateManager->GetContext()->mWindow->Draw(mNewGame);
	mStateManager->GetContext()->mWindow->Draw(mExit);
}

void State_GameOver::Update(const sf::Time & lTime)
{
	if (mOption == 0) {
		mNewGame.setFillColor(sf::Color::Red);
		mExit.setFillColor(sf::Color::White);
	}
	else if (mOption == 1) {
		mNewGame.setFillColor(sf::Color::White);
		mExit.setFillColor(sf::Color::Red);
	}
}

void State_GameOver::ChangeOptionLeft(EventDetails * lDetails)
{
	if (mOption != 0)
		--mOption;
}

void State_GameOver::ChangeOptionRight(EventDetails * lDetails)
{
	if (mOption != 1)
		++mOption;
}

void State_GameOver::Enter(EventDetails * lDetails)
{
	if (mOption == 0)
	{
		mStateManager->SwitchTo(StateType::Maze);
	}
	else if (mOption == 1)
	{
		mStateManager->GetContext()->mWindow->EndProgram(nullptr);
	}
}
