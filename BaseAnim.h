#pragma once
#include <SFML/Graphics.hpp>
#include <sstream>

class SpriteSheet;
using Frame = unsigned int;

class BaseAnim
{
	friend SpriteSheet;
public:
	BaseAnim();
	virtual ~BaseAnim();

	void SetSpriteSheet(SpriteSheet* lSheet);
	void SetFrame(const Frame& lFrame);
	void SetStartFrame(Frame lFrame);
	void SetEndFrame(const Frame& lFrame);
	void SetFrameRow(const Frame& lRow);
	void SetActionStart(const int& lFrame);
	void SetActionEnd(const int& lFrame);
	void SetFrameTime(const float& lSeconds);
	void SetLooping(const bool& lLoop);
	void SetName(const std::string& lName);

	SpriteSheet* GetSpriteSheet();
	Frame GetFrame();
	Frame GetStartFrame();
	Frame GetEndFrame();
	Frame GetFrameRow();
	int GetActionStart();
	int GetActionEnd();
	float GetFrameTime();
	sf::Time GetElapsedTime();
	bool IsLooping();
	bool IsPlaying();
	bool IsInAction();
	std::string GetName();

	void Play();
	void Pause();
	void Stop();
	void Reset();

	virtual void Update(const sf::Time& lTime);
	
	friend std::stringstream& operator>>(std::stringstream& lStream, BaseAnim& a)
	{
		a.ReadIn(lStream);
		return lStream;
	}
protected:
	virtual void Framestep() = 0;
	virtual void CropSprite() = 0;
	virtual void ReadIn(std::stringstream& lStream) = 0;

	Frame mFrameCurrent;
	Frame mFrameStart;
	Frame mFrameEnd;
	Frame mFrameRow;
	float mFrameTime;
	sf::Time mElapsedTime;
	bool mLoop;
	bool mPlaying;
	std::string mName;
	int mFrameActionStart;
	int mFrameActionEnd;
	SpriteSheet* mSpriteSheet;
};