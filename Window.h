#pragma once
#include "EventManager.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

class Window
{
public:
	Window();
	~Window();

	bool IsDone();

	void Draw(const sf::Drawable &lSprite);
	void BeginDrawing();
	void EndDrawing();
	void ToggleFullscreen(EventDetails* lDetails);
	void EndProgram(EventDetails* lDetails);

	void Update();

	sf::FloatRect GetViewSpace();
	sf::View& GetDefaultView();
	EventManager* GetEventManager();
	sf::RenderWindow* GetRenderWindow();
private:
	void Open();
	void Close();

	bool mIsDone;
	bool mIsFullscreen;
	sf::RenderWindow mWindow;
	const sf::Vector2i mResolution;
	sf::View mDefaultView;
	EventManager mEventManager;
};