#include "Player.h"
#include "StateManager.h"

Player::Player(TextureManager* lTex)
	:mMaxVelocity(128.f), mSpeed(2048.f), mBaseFriction(16.f), mSpriteSheet(lTex),
	mVelocity(0, 0)
{
	mSpriteSheet.LoadSheet("spritesheets/player.sheet");
	mSpriteSheet.SetAnimation("Walk", true, true);
	UpdateAABB();
}

Player::~Player()
{
}

void Player::SetMap(Maze * lMaze)
{
	mMaze = lMaze;
}

void Player::SetStateManager(StateManager * lState)
{
	mStateManager = lState;
}

void Player::SetPosition(const sf::Vector2f& lPos)
{
	mPosition = lPos;
	mSpriteSheet.SetSpritePosition(sf::Vector2f(mPosition.x, mPosition.y));
}

sf::Vector2f Player::GetPosition()
{
	return mPosition;
}

void Player::Accelerate(const Dir& lDir)
{
	if (lDir == Dir::Left)
	{
		mAcceleration += sf::Vector2f(-mSpeed, 0);
		mSpriteSheet.SetSpriteDirection(Dir::Left);
	}
	else if (lDir == Dir::Right)
	{
		mAcceleration += sf::Vector2f(mSpeed, 0);
		mSpriteSheet.SetSpriteDirection(Dir::Right);
	}
	else if (lDir == Dir::Up)
	{
		mAcceleration += sf::Vector2f(0, -mSpeed);
	}
	else if (lDir == Dir::Down)
	{
		mAcceleration += sf::Vector2f(0, mSpeed);
	}
}

void Player::Move(const float & x, const float & y)
{
	mPosition.x += x;
	mPosition.y += y;
	mSpriteSheet.SetSpritePosition(sf::Vector2f(mPosition.x, mPosition.y));
	UpdateAABB();
}

void Player::Draw(sf::RenderWindow * lWind)
{
	mSpriteSheet.Draw(lWind);
}

void Player::Update(const sf::Time & lTime)
{
	ApplyFriction();
	mVelocity += mAcceleration * lTime.asSeconds();
	
	float currentVelocity = sqrt(pow(mVelocity.x, 2) + pow(mVelocity.y, 2));
	if (currentVelocity > mMaxVelocity)
	{
		if (mVelocity.x == 0)
			mVelocity.y = mMaxVelocity * abs(mVelocity.y) / mVelocity.y;
		else if (mVelocity.y == 0)
			mVelocity.x = mMaxVelocity * abs(mVelocity.x) / mVelocity.x;
		else
		{
			mVelocity.x = mMaxVelocity * mVelocity.x / currentVelocity;
			mVelocity.y = mMaxVelocity * mVelocity.y / currentVelocity;
		}
	}

	Move(mVelocity.x*lTime.asSeconds(), mVelocity.y*lTime.asSeconds());
	if (mVelocity.x != 0 || mVelocity.y != 0)
	{
		mSpriteSheet.SetAnimation("Walk", true, true);
	}
	else if(mSpriteSheet.GetCurrentAnim()->GetName() != "Idle")
	{
		mSpriteSheet.SetAnimation("Idle", true, true);
	}
	mSpriteSheet.Update(lTime);
	mAcceleration.x = 0;
	mAcceleration.y = 0;

	UpdateAABB();
	CheckForCollisions();
	ResolveCollisions();
}

void Player::UpdateAABB()
{
	mAABB.left = mSpriteSheet.GetSpritePosition().x - mSpriteSheet.GetSpriteSize().x / 4;
	mAABB.width = mSpriteSheet.GetSpriteSize().x / 2;
	mAABB.top = mSpriteSheet.GetSpritePosition().y + mSpriteSheet.GetSpriteSize().y / 4;
	mAABB.height = mSpriteSheet.GetSpriteSize().y / 4;
}

void Player::ApplyFriction()
{
	if (mVelocity.x != 0)
	{
		if (abs(mVelocity.x) - abs(mBaseFriction) < 0) { mVelocity.x = 0; }
		else
		{
			if (mVelocity.x < 0) { mVelocity.x += mBaseFriction; }
			else { mVelocity.x -= mBaseFriction; }
		}
	}
	if (mVelocity.y != 0)
	{
		if (abs(mVelocity.y) - abs(mBaseFriction) < 0) { mVelocity.y = 0; }
		else
		{
			if (mVelocity.y < 0) { mVelocity.y += mBaseFriction; }
			else { mVelocity.y -= mBaseFriction; }
		}
	}
}

void Player::CheckForCollisions()
{
	int tileSize = mMaze->GetTileSize();
	int fromX = floor(mAABB.left / tileSize);
	int toX = floor((mAABB.left + mAABB.width) / tileSize);
	int fromY = floor(mAABB.top / tileSize);
	int toY = floor((mAABB.top + mAABB.height) / tileSize);

	for (int x = fromX; x <= toX; ++x)
	{
		for (int y = fromY; y <= toY; ++y)
		{
			TileInfo* tile = mMaze->GetTile(x, y);
			if (!tile) continue;
			sf::FloatRect tileBounds(x*tileSize, y*tileSize, tileSize, tileSize);
			sf::FloatRect intersection;
			mAABB.intersects(tileBounds, intersection);
			float area = intersection.width * intersection.height;
			CollisionElement col;
			col.mTile = tile;
			col.mArea = area;
			col.mTileBounds = tileBounds;
			mCollisions.push_back(col);
		}
	}
}

void Player::ResolveCollisions()
{
	if (mCollisions.empty()) { return; }
	std::sort(mCollisions.begin(), mCollisions.end(), mComp);
	int tileSize = mMaze->GetTileSize();
	for (int i = 0; i < mCollisions.size(); ++i)
	{
		if (mCollisions[i].mTile->mIsEnd == true) { mStateManager->SwitchTo(StateType::GameOver); }
		if (mCollisions[i].mTile->mIsCollidable == false) { continue; }
		if (!mAABB.intersects(mCollisions[i].mTileBounds)) { continue; }
		float xDiff = (mAABB.left + (mAABB.width / 2)) - (mCollisions[i].mTileBounds.left + (mCollisions[i].mTileBounds.width / 2));
		float yDiff = (mAABB.top + (mAABB.height / 2)) - (mCollisions[i].mTileBounds.top + (mCollisions[i].mTileBounds.height / 2));
		float resolve = 0;
		if (abs(xDiff) > abs(yDiff))
		{
			if (xDiff > 0)
			{
				resolve = mCollisions[i].mTileBounds.left + tileSize - mAABB.left;
			}
			else
			{
				resolve = -((mAABB.left + mAABB.width) - mCollisions[i].mTileBounds.left);
			}
			Move(resolve, 0);
			mVelocity.x = 0;
		}
		else
		{
			if (yDiff > 0)
			{
				resolve = mCollisions[i].mTileBounds.top + tileSize - mAABB.top;
			}
			else
			{
				resolve = -((mAABB.top + mAABB.height) - mCollisions[i].mTileBounds.top);
			}
			Move(0, resolve);
			mVelocity.y = 0;
		}
	}
	mCollisions.clear();
}
