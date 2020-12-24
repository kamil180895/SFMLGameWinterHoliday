#include "Anim_Directional.h"
#include "SpriteSheet.h"

void Anim_Directional::Framestep()
{
	if (mFrameStart < mFrameEnd)
	{
		++mFrameCurrent;
	}
	else
	{
		--mFrameCurrent;
	}

	if ((mFrameStart < mFrameEnd && mFrameCurrent > mFrameEnd) || (mFrameStart > mFrameEnd && mFrameCurrent < mFrameEnd))
	{
		if (mLoop)
		{
			mFrameCurrent = mFrameStart;
			return;
		}
		mFrameCurrent = mFrameEnd;
		Pause();
	}
}

void Anim_Directional::CropSprite()
{
	sf::IntRect rect;
	rect.left = mSpriteSheet->GetSpriteSize().x * mFrameCurrent;
	rect.top = mSpriteSheet->GetSpriteSize().y * (mFrameRow + (int)mSpriteSheet->GetDirection());
	rect.width = mSpriteSheet->GetSpriteSize().x;
	rect.height = mSpriteSheet->GetSpriteSize().y;
	mSpriteSheet->CropSprite(rect);
}

void Anim_Directional::ReadIn(std::stringstream & lStream)
{
	lStream >> mFrameStart >> mFrameEnd >> mFrameRow >> mFrameTime >> mFrameActionStart >> mFrameActionEnd;
}
