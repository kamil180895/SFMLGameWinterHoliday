#include "SpriteSheet.h"

SpriteSheet::SpriteSheet(TextureManager * lTexManager)
	:mTextureManager(lTexManager), mCurrentAnimation(nullptr), mSpriteScale(1.f , 1.f),
	mDirection(Dir::Right)
{
}

SpriteSheet::~SpriteSheet()
{
	ReleaseSheet();
}

void SpriteSheet::CropSprite(const sf::IntRect & lRect)
{
	mSprite.setTextureRect(lRect);
}

sf::Vector2i SpriteSheet::GetSpriteSize() const
{
	return mSpriteSize;
}

sf::Vector2f SpriteSheet::GetSpritePosition() const
{
	return mSprite.getPosition();
}

Dir SpriteSheet::GetDirection()
{
	return mDirection;
}

void SpriteSheet::SetSpriteSize(const sf::Vector2i & lSize)
{
	mSpriteSize = lSize;
	mSprite.setOrigin(mSpriteSize.x / 2, mSpriteSize.y / 2);
}

void SpriteSheet::SetSpritePosition(const sf::Vector2f & lPos)
{
	mSprite.setPosition(lPos);
}

void SpriteSheet::SetSpriteDirection(const Dir & lDir)
{
	if (lDir == mDirection) { return; }
	mDirection = lDir;
	mCurrentAnimation->CropSprite();
}

bool SpriteSheet::LoadSheet(const std::string & lPath)
{
	std::ifstream file;
	file.open(lPath);
	if (!file.is_open())
	{
		std::cout << "Error: Unable to open " << lPath << std::endl;
		return false;
	}

	ReleaseSheet();
	std::string line;
	while (std::getline(file, line))
	{
		std::stringstream keystream(line);
		std::string type;
		keystream >> type;
		if (type == "Texture")
		{
			if (mTexture != "")
			{
				std::cout << "Error: Duplicate texture entries in " << lPath << std::endl;
				continue;
			}
			std::string texture;
			keystream >> texture;
			if (!mTextureManager->RequireResource(texture))
			{
				std::cout << "Error: Could not set up texture: " << texture << std::endl;
				continue;
			}
			mTexture = texture;
			mSprite.setTexture(*mTextureManager->GetResource(mTexture));
		}
		else if (type == "Size")
		{
			keystream >> mSpriteSize.x >> mSpriteSize.y;
			SetSpriteSize(mSpriteSize);
		}
		else if (type == "Scale")
		{
			keystream >> mSpriteScale.x >> mSpriteScale.y;
			mSprite.setScale(mSpriteScale);
		}
		else if (type == "AnimationType")
		{
			keystream >> mAnimType;
		}
		else if (type == "Animation")
		{
			std::string name;
			keystream >> name;
			if (mAnimations.find(name) != mAnimations.end())
			{
				std::cout << "Error: Duplicate animation(" << name << ") in " << lPath << std::endl;
				continue;
			}
			BaseAnim* anim = nullptr;
			if (mAnimType == "Directional")
			{
				anim = new Anim_Directional;
			}//place for new animation types
			else
			{
				std::cout << "Error: Unknown animation type: " << mAnimType << " in " << lPath << std::endl;
				continue;
			}

			keystream >> *anim;
			anim->SetSpriteSheet(this);
			anim->SetName(name);
			anim->Reset();
			mAnimations.emplace(name, anim);

			if (mCurrentAnimation) { continue; }
			mCurrentAnimation = anim;
			mCurrentAnimation->Play();
		}
	}
	file.close();
	return true;
}

void SpriteSheet::ReleaseSheet()
{
	mTextureManager->ReleaseResource(mTexture);
	mCurrentAnimation = nullptr;
	while (mAnimations.begin() != mAnimations.end())
	{
		delete mAnimations.begin()->second;
		mAnimations.erase(mAnimations.begin());
	}
}

BaseAnim * SpriteSheet::GetCurrentAnim()
{
	return mCurrentAnimation;
}

bool SpriteSheet::SetAnimation(const std::string & lName, const bool & lPlay, const bool & lLoop)
{
	auto itr = mAnimations.find(lName);
	if (itr == mAnimations.end()) { return false; }
	if (itr->second == mCurrentAnimation) { return false; }
	if (mCurrentAnimation) { mCurrentAnimation->Stop(); }
	mCurrentAnimation = itr->second;
	mCurrentAnimation->SetLooping(lLoop);
	if (lPlay) { mCurrentAnimation->Play(); }
	mCurrentAnimation->CropSprite();
	return true;
}

void SpriteSheet::Update(const sf::Time& lTime)
{
	mCurrentAnimation->Update(lTime);
}

void SpriteSheet::Draw(sf::RenderWindow * lWind)
{
	lWind->draw(mSprite);
}
