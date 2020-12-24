#include "State_Credits.h"
#include "StateManager.h"

State_Credits::State_Credits(StateManager * lStateManager)
	:BaseState(lStateManager)
{
}

State_Credits::~State_Credits()
{
}

void State_Credits::OnCreate() {
	mStateManager->GetContext()->mTextureManager->RequireResource("CreditsPlaceholder");
	sf::Texture* tex = mStateManager->GetContext()->mTextureManager->GetResource("CreditsPlaceholder");
	SCreditsBackground.setTexture(*tex);

	mStateManager->GetContext()->mFontManager->RequireResource("MainFont");
	sf::Font* MainFont = mStateManager->GetContext()->mFontManager->GetResource("MainFont");

	mStateManager->GetContext()->mEventManager->AddCallback(StateType::Credits, "Enter", &State_Credits::GoBackMenu, this);

	exit.setFont(*MainFont);
	exit.setString(">BACK<");
	exit.setFillColor(sf::Color::Red);
	exit.setCharacterSize(30);
	exit.setPosition(sf::Vector2f(126.0, 320.0));
}

void State_Credits::OnDestroy(){
	mStateManager->GetContext()->mTextureManager->ReleaseResource("CreditsPlaceholder");
	mStateManager->GetContext()->mFontManager->ReleaseResource("MainFont");
	mStateManager->GetContext()->mEventManager->RemoveCallback(StateType::Credits, "Enter");
}

void State_Credits::Activate()
{
}

void State_Credits::Deactivate()
{
}

void State_Credits::Draw(){
	mStateManager->GetContext()->mWindow->Draw(SCreditsBackground);
	mStateManager->GetContext()->mWindow->Draw(exit);
}

void State_Credits::Update(const sf::Time & lTime)
{
}

void State_Credits::GoBackMenu(EventDetails* lDetails) {
	mStateManager->SwitchTo(StateType::Menu);
}
