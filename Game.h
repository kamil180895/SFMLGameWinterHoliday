#pragma once
#include "Window.h"
#include "SharedContext.h"
#include "StateManager.h"
#include "TextureManager.h"
#include "FontManager.h"

class Game
{
public:
	Game();
	~Game();

	bool IsDone();

	void Draw();
	void Update();
	void LateUpdate();

	sf::Time GetElapsed();

	Window* GetWindow();
private:
	void RestartClock();

	sf::Clock mClock;
	sf::Time mElapsed;
	Window mWindow;
	SharedContext mContext;
	TextureManager mTextureManger;
	FontManager mFontManager;
	StateManager mStateManager;
};