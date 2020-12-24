#include "State_Menu.h"
#include "StateManager.h"

State_Menu::State_Menu(StateManager * lStateManager)
	:BaseState(lStateManager)
{
	mCurrentOption = 1;
}

State_Menu::~State_Menu()
{
}

void State_Menu::OnCreate(){

	mStateManager->GetContext()->mTextureManager->RequireResource("MenuPlaceholder");
	sf::Texture* tex = mStateManager->GetContext()->mTextureManager->GetResource("MenuPlaceholder");
	SMainMenuBackground.setTexture(*tex);

	mStateManager->GetContext()->mFontManager->RequireResource("MainFont");
	sf::Font* MainFont = mStateManager->GetContext()->mFontManager->GetResource("MainFont");

	mStateManager->GetContext()->mEventManager->AddCallback(StateType::Menu, "W", &State_Menu::ChangeOptionUp, this);
	mStateManager->GetContext()->mEventManager->AddCallback(StateType::Menu, "S", &State_Menu::ChangeOptionDown, this);
	mStateManager->GetContext()->mEventManager->AddCallback(StateType::Menu, "Enter", &State_Menu::ChooseOptionEnter, this);

	play.setFont(*MainFont);
	play.setString(">PLAY<");
	play.setCharacterSize(30);
	play.setPosition(sf::Vector2f(120.0, 170.0));

	credits.setFont(*MainFont);
	credits.setString(">CREDITS<");
	credits.setCharacterSize(30);
	credits.setPosition(sf::Vector2f(100.0, 220.0));

	exit.setFont(*MainFont);
	exit.setString(">EXIT<");
	exit.setCharacterSize(30);
	exit.setPosition(sf::Vector2f(126.0, 320.0));

}

void State_Menu::OnDestroy()
{
	mStateManager->GetContext()->mTextureManager->ReleaseResource("MenuPlaceholder");
	mStateManager->GetContext()->mFontManager->ReleaseResource("MainFont");
	mStateManager->GetContext()->mEventManager->RemoveCallback(StateType::Menu, "W");
	mStateManager->GetContext()->mEventManager->RemoveCallback(StateType::Menu, "S");
	mStateManager->GetContext()->mEventManager->RemoveCallback(StateType::Menu, "Enter");
}

void State_Menu::Activate()
{
}

void State_Menu::Deactivate()
{
}

void State_Menu::Draw(){
	mStateManager->GetContext()->mWindow->Draw(SMainMenuBackground);
	mStateManager->GetContext()->mWindow->Draw(play);
	mStateManager->GetContext()->mWindow->Draw(credits);
	mStateManager->GetContext()->mWindow->Draw(exit);
}

void State_Menu::Update(const sf::Time & lTime)
{
	if (mCurrentOption == 1) {
		play.setFillColor(sf::Color::Red);
		credits.setFillColor(sf::Color::White);
		exit.setFillColor(sf::Color::White);
	}
	else if (mCurrentOption == 2) {
		play.setFillColor(sf::Color::White);
		credits.setFillColor(sf::Color::Red);
		exit.setFillColor(sf::Color::White);
	}
	else if (mCurrentOption == 3) {
		play.setFillColor(sf::Color::White);
		credits.setFillColor(sf::Color::White);
		exit.setFillColor(sf::Color::Red);
	}
}

void State_Menu::ChangeOptionDown(EventDetails* lDetails) {
	if (mCurrentOption != 3) {
		mCurrentOption += 1;
	}
}

void State_Menu::ChooseOptionEnter(EventDetails* lDetails) {
	switch (mCurrentOption) {
	case 1:
		mStateManager->SwitchTo(StateType::Maze);
		break;

	case 2:
		mStateManager->SwitchTo(StateType::Credits);
		break;

	case 3:
		mStateManager->GetContext()->mWindow->EndProgram(nullptr);
		break;
	}
}

void State_Menu::ChangeOptionUp(EventDetails* lDetails) {
	if (mCurrentOption != 1) {
		mCurrentOption -= 1;
	}
}

