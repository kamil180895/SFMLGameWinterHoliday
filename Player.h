#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>
#include "TextureManager.h"
#include "Maze.h"
#include "SpriteSheet.h"
#include "Direction.h"



struct CollisionElement
{
	float mArea;
	TileInfo* mTile;
	sf::FloatRect mTileBounds;
};


struct CollisionsComparator
{
	bool operator() (const CollisionElement& l1, const CollisionElement& l2) { return l1.mArea > l2.mArea; }
};

class StateManager;
class Player
{
public:
	Player(TextureManager* lTex);
	~Player();

	void SetMap(Maze* lMaze);
	void SetStateManager(StateManager* lState);

	void SetPosition(const sf::Vector2f& lPos);
	sf::Vector2f GetPosition();
	void Accelerate(const Dir& lDir);
	void Move(const float&x, const float&y);

	void Draw(sf::RenderWindow* lWind);
	void Update(const sf::Time& lTime);
private:
	void UpdateAABB();
	void ApplyFriction();
	void CheckForCollisions();
	void ResolveCollisions();

	SpriteSheet mSpriteSheet;
	std::string mTexture;
	sf::Vector2f mAcceleration;
	sf::Vector2f mVelocity;
	sf::Vector2f mPosition;
	float mSpeed;
	float mMaxVelocity;
	float mBaseFriction;

	sf::FloatRect mAABB;
	std::vector<CollisionElement> mCollisions;

	Maze* mMaze;
	TextureManager* mTextureManager;
	StateManager* mStateManager;
	CollisionsComparator mComp;
};