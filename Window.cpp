#include "Window.h"

Window::Window()
	: mResolution(854, 480), mIsFullscreen(false), mIsDone(false)
{
	mDefaultView.setCenter(mResolution.x / 2, mResolution.y / 2);
	mDefaultView.setSize(mResolution.x, mResolution.y);
	mEventManager.AddCallback(StateType(0), "Close", &Window::EndProgram, this);
	mEventManager.AddCallback(StateType(0), "ToggleFullscreen", &Window::ToggleFullscreen, this);
	Open();
}

Window::~Window()
{
	Close();
}

void Window::Open()
{
	sf::Uint32 style = { mIsFullscreen ? (sf::Uint32)sf::Style::Fullscreen : (sf::Uint32)sf::Style::Titlebar | sf::Style::Close};
	mWindow.create(sf::VideoMode(mResolution.x, mResolution.y, 32), "MazeGame", style);
	mWindow.setFramerateLimit(60);
}

void Window::Close()
{
	mWindow.close();
}

bool Window::IsDone()
{
	return mIsDone;
}

void Window::Draw(const sf::Drawable & lSprite)
{
	mWindow.draw(lSprite);
}

void Window::BeginDrawing()
{
	mWindow.clear(sf::Color::Black);
}

void Window::EndDrawing()
{
	mWindow.display();
}

void Window::ToggleFullscreen(EventDetails* lDetails)
{
	mIsFullscreen = !mIsFullscreen;
	Close();
	Open();
}

void Window::EndProgram(EventDetails* lDetails)
{
	mIsDone = true;
}

void Window::Update()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		if (event.type == sf::Event::GainedFocus)
		{
			mEventManager.SetFocus(true);
		}
		else if (event.type == sf::Event::LostFocus)
		{
			mEventManager.SetFocus(false);
		}

		mEventManager.HandleEvent(event);
	}
	mEventManager.Update();
}

sf::FloatRect Window::GetViewSpace()
{
	sf::Vector2f viewCenter(mWindow.getView().getCenter());
	sf::Vector2f viewSize(mWindow.getView().getSize());
	sf::Vector2f viewSizeHalf(viewSize.x / 2, viewSize.y / 2);
	sf::FloatRect viewSpace(viewCenter - viewSizeHalf, viewSize);
	return viewSpace;
}

sf::View & Window::GetDefaultView()
{
	return mDefaultView;
}

EventManager * Window::GetEventManager()
{
	return &mEventManager;
}

sf::RenderWindow * Window::GetRenderWindow()
{
	return &mWindow;
}
