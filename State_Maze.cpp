#include "State_Maze.h"
#include "StateManager.h"

State_Maze::State_Maze(StateManager * lStateManager)
	:BaseState(lStateManager), mZoom(0.4), mDebugMode(false), mPlayer(lStateManager->GetContext()->mTextureManager)
{
}

State_Maze::~State_Maze()
{
}

void State_Maze::OnCreate()
{
	mStateManager->GetContext()->mEventManager->AddCallback(StateType::Maze, "RealTimeW", &State_Maze::GoUp, this);
	mStateManager->GetContext()->mEventManager->AddCallback(StateType::Maze, "RealTimeS", &State_Maze::GoDown, this);
	mStateManager->GetContext()->mEventManager->AddCallback(StateType::Maze, "RealTimeD", &State_Maze::GoRight, this);
	mStateManager->GetContext()->mEventManager->AddCallback(StateType::Maze, "RealTimeA", &State_Maze::GoLeft, this);
	mStateManager->GetContext()->mEventManager->AddCallback(StateType::Maze, "H", &State_Maze::ToggleDebug, this);
	mMaze.SetTextureManager(mStateManager->GetContext()->mTextureManager);
	mMaze.SetupTileSet();
	mMaze.LoadFragments();
	mMaze.GenerateNewMaze();
	mPlayer.SetMap(&mMaze);
	mPlayer.SetStateManager(mStateManager);
	mPlayer.SetPosition(mMaze.GetStartingPos());
	mStateManager->GetContext()->mTextureManager->RequireResource("Filter");
	mFilter.setTexture(*mStateManager->GetContext()->mTextureManager->GetResource("Filter"));
	mView.zoom(mZoom);
}

void State_Maze::OnDestroy()
{
	mStateManager->GetContext()->mEventManager->RemoveCallback(StateType::Maze, "RealTimeW");
	mStateManager->GetContext()->mEventManager->RemoveCallback(StateType::Maze, "RealTimeA");
	mStateManager->GetContext()->mEventManager->RemoveCallback(StateType::Maze, "RealTimeS");
	mStateManager->GetContext()->mEventManager->RemoveCallback(StateType::Maze, "RealTimeD");
	mStateManager->GetContext()->mEventManager->RemoveCallback(StateType::Maze, "H");
	mStateManager->GetContext()->mTextureManager->ReleaseResource("Filter");
	mMaze.PurgeTileSet();
}

void State_Maze::Activate()
{
}

void State_Maze::Deactivate()
{
}

void State_Maze::Draw()
{
	sf::RenderWindow* renderWindow = mStateManager->GetContext()->mWindow->GetRenderWindow();
	mMaze.Draw(renderWindow, mStateManager->GetContext()->mWindow->GetViewSpace());
	mPlayer.Draw(renderWindow);

	if (mDebugMode == false)
	{
		renderWindow->setView(mStateManager->GetContext()->mWindow->GetDefaultView());
		renderWindow->draw(mFilter);
		renderWindow->setView(mView);
	}
}

void State_Maze::Update(const sf::Time & lTime)
{
	mPlayer.Update(lTime);
	mView.setCenter(mPlayer.GetPosition());
}

void State_Maze::GoUp(EventDetails * lDetails)
{
	mPlayer.Accelerate(Dir::Up);
}

void State_Maze::GoDown(EventDetails * lDetails)
{
	mPlayer.Accelerate(Dir::Down);
}

void State_Maze::GoLeft(EventDetails * lDetails)
{
	mPlayer.Accelerate(Dir::Left);
}

void State_Maze::GoRight(EventDetails * lDetails)
{
	mPlayer.Accelerate(Dir::Right);
}

void State_Maze::ToggleDebug(EventDetails * lDetails)
{
	if (mDebugMode == false)
	{
		mView.zoom(10.f / 4);
	}
	else
		mView.zoom(0.4);

	mDebugMode = !mDebugMode;
}
