#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>
#include <queue>
#include "TextureManager.h"

#define TileSize 32 //in pixels
#define FragmentWidth 4 //in tiles
#define FragmentHeight 5 //in tiles

enum class Entrance
{
	Left, Right, Up, Down
};

struct TileInfo
{
	TileInfo() :mTexture(""), mIsCollidable(false), mIsEnd(false) { }

	std::string mTexture;
	sf::Sprite mSprite;
	bool mIsCollidable;
	bool mIsEnd;
};

struct Fragment
{
	TileInfo* mTile[FragmentHeight][FragmentWidth];
};

class Maze
{
public:
	Maze();
	~Maze();
	
	void GenerateNewMaze();
	void Draw(sf::RenderWindow* lWind, const sf::FloatRect& lView);

	void SetTextureManager(TextureManager* lTex);
	void SetupTileSet();
	void PurgeTileSet();
	void LoadFragments();
	void PurgeFragments();
	sf::Vector2f GetStartingPos();
	int GetTileSize();
	TileInfo* GetTile(const int&x, const int&y);
private:
	void InitializeVector();
	void GenerateMazeProper(int x, int y, const Entrance& lEntry);
	void InsertFragment(const int& x, const int& y, Fragment* lFragment);
	void FillBoundaries();
	void AddWallsWhereNeeded();
	void PlaceEndPortal(int x, int y);
	
	std::vector<Fragment*> mFragments;//dynamic allocation
	std::unordered_map<Entrance, std::vector<Fragment*>> mFragmentsWithEntrance;//not allocation
	std::vector<TileInfo*> mTileSet;//dynamic allocation

	std::vector<std::vector<TileInfo*>> mMap;
	sf::Vector2f mStart;

	const int mMazeSize;//in fragments
	TextureManager* mTextureManager;
};