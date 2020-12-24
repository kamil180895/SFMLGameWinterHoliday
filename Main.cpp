#include "Game.h"

int main()
{
	Game mGame;
	while (!mGame.IsDone())
	{
		mGame.Draw();
		mGame.Update();
		mGame.LateUpdate();
	}
	return 0;
}