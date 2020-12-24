#include "BaseAnim.h"
#include "SpriteSheet.h"

BaseAnim::BaseAnim()
	: mFrameCurrent(0), mFrameStart(0), mFrameEnd(0), mFrameRow(0),
	mFrameTime(0.f), mLoop(false), mPlaying(false), mFrameActionStart(-1), mFrameActionEnd(-1)
{
	mElapsedTime = sf::seconds(0.f);
}

BaseAnim::~BaseAnim()
{
}

void BaseAnim::SetSpriteSheet(SpriteSheet * lSheet)
{
	mSpriteSheet = lSheet;
}

void BaseAnim::SetFrame(const Frame& lFrame)
{
	mFrameCurrent = lFrame;
}

void BaseAnim::SetStartFrame(Frame lFrame)
{
	mFrameStart = lFrame;
}

void BaseAnim::SetEndFrame(const Frame & lFrame)
{
	mFrameEnd = lFrame;
}

void BaseAnim::SetFrameRow(const Frame & lRow)
{
	mFrameRow = lRow;
}

void BaseAnim::SetActionStart(const int & lFrame)
{
	mFrameActionStart = lFrame;
}

void BaseAnim::SetActionEnd(const int & lFrame)
{
	mFrameActionEnd = lFrame;
}

void BaseAnim::SetFrameTime(const float & lSeconds)
{
	mFrameTime = lSeconds;
}

void BaseAnim::SetLooping(const bool & lLoop)
{
	mLoop = lLoop;
}

void BaseAnim::SetName(const std::string & lName)
{
	mName = lName;
}

SpriteSheet * BaseAnim::GetSpriteSheet()
{
	return mSpriteSheet;
}

Frame BaseAnim::GetFrame()
{
	return mFrameCurrent;
}

Frame BaseAnim::GetStartFrame()
{
	return mFrameStart;
}

Frame BaseAnim::GetEndFrame()
{
	return mFrameEnd;
}

Frame BaseAnim::GetFrameRow()
{
	return mFrameRow;
}

int BaseAnim::GetActionStart()
{
	return mFrameActionStart;
}

int BaseAnim::GetActionEnd()
{
	return mFrameActionEnd;
}

float BaseAnim::GetFrameTime()
{
	return mFrameTime;
}

sf::Time BaseAnim::GetElapsedTime()
{
	return mElapsedTime;
}

bool BaseAnim::IsLooping()
{
	return mLoop;
}

bool BaseAnim::IsPlaying()
{
	return mPlaying;
}

bool BaseAnim::IsInAction()
{
	if (mFrameActionStart == -1 || mFrameActionEnd == -1)
		return true;

	if (mFrameCurrent >= mFrameActionStart && mFrameCurrent <= mFrameActionEnd)
		return true;
	else
		return false;
}

std::string BaseAnim::GetName()
{
	return mName;
}

void BaseAnim::Play()
{
	mPlaying = true;
}

void BaseAnim::Pause()
{
	mPlaying = false;
}

void BaseAnim::Stop()
{
	mPlaying = false;
	Reset();
}

void BaseAnim::Reset()
{
	mFrameCurrent = mFrameStart;
	mElapsedTime = sf::seconds(0.f);
	CropSprite();
}

void BaseAnim::Update(const sf::Time & lTime)
{
	if (!mPlaying) { return; }
	mElapsedTime += lTime;
	if (mElapsedTime.asSeconds() < mFrameTime) { return; }
	Framestep();
	CropSprite();
	mElapsedTime = sf::seconds(0.f);
}
