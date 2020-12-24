#include "Game.h"

Game::Game()
	:mStateManager(&mContext)
{
	srand(time(NULL));
	mClock.restart();

	mContext.mWindow = &mWindow;
	mContext.mEventManager = mWindow.GetEventManager();
	mContext.mTextureManager = &mTextureManger;
	mContext.mFontManager = &mFontManager;

	mStateManager.SwitchTo(StateType::Menu);
}

Game::~Game()
{
}

bool Game::IsDone()
{
	return mWindow.IsDone();
}

void Game::Draw()
{
	mWindow.BeginDrawing();
	mStateManager.Draw();
	mWindow.EndDrawing();
}

void Game::Update()
{
	mStateManager.Update(mElapsed);
	mWindow.Update();
}

void Game::LateUpdate()
{
	mStateManager.ProcessRequests();
	RestartClock();
}

sf::Time Game::GetElapsed()
{
	return mElapsed;
}

void Game::RestartClock()
{
	mElapsed = mClock.restart();
}

Window * Game::GetWindow()
{
	return &mWindow;
}
