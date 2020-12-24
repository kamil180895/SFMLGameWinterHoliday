#include "Maze.h"

Maze::Maze()
	: mMazeSize(11)
{
}

Maze::~Maze()
{
}

void Maze::GenerateNewMaze()
{
	InitializeVector();
	GenerateMazeProper(mMazeSize*FragmentWidth / 2, mMazeSize*FragmentHeight / 2, Entrance::Left);
	mStart.x = 1.f*(mMazeSize / 2 + 1)*FragmentWidth*TileSize + FragmentWidth * TileSize / 2;
	mStart.y = 1.f*(mMazeSize / 2 + 1)*FragmentHeight*TileSize + FragmentHeight * TileSize / 2;
	FillBoundaries();
	AddWallsWhereNeeded();
	PlaceEndPortal(floor(mStart.x / TileSize), floor(mStart.y / TileSize));
}

void Maze::Draw(sf::RenderWindow * lWind, const sf::FloatRect& lView)
{
	for (int i = 0; i < mMap.size(); ++i)
	{
		for (int j = 0; j < mMap[i].size(); ++j)
		{
			if (mMap[i][j] == nullptr)
				continue;
			mMap[i][j]->mSprite.setPosition(j*TileSize, i*TileSize);
			if(lView.intersects(mMap[i][j]->mSprite.getGlobalBounds()))
				lWind->draw(mMap[i][j]->mSprite);
		}
	}
}

void Maze::SetTextureManager(TextureManager * lTex)
{
	mTextureManager = lTex;
}

void Maze::LoadFragments()
{
	mFragments.clear();
	mFragmentsWithEntrance.clear();
	const int numberOfFiles = 11;
	Fragment* fragment;
	std::ifstream file;
	std::string texName;
	sf::Texture* tex;
	std::string names[numberOfFiles] = {"NSWE", "NSW", "NES", "NEW", "ESW", "NE", "NW", "ES", "SW", "EW", "NS"};

	for (int i = 0; i < numberOfFiles; ++i)
	{
		file.open("fragments/" + names[i] + ".frag");
		if (file.is_open() == false)
		{
			std::cout << "Error: Cannot open " << names[i] << ".png\n";
			continue;
		}

		fragment = new Fragment;
		for (int j = 0; j < FragmentHeight; ++j)
		{
			for (int k = 0; k < FragmentWidth; ++k)
			{
				std::getline(file, texName);
				auto itr = std::find_if(mTileSet.begin(), mTileSet.end(), [texName](TileInfo* lTile) {
					return lTile->mTexture == texName;
				});
				fragment->mTile[j][k] = *itr;
			}
		}
		mFragments.push_back(fragment);

		if(names[i].find('S') != std::string::npos)
			mFragmentsWithEntrance[Entrance::Down].push_back(fragment);
		if (names[i].find('N') != std::string::npos)
			mFragmentsWithEntrance[Entrance::Up].push_back(fragment);
		if (names[i].find('W') != std::string::npos)
			mFragmentsWithEntrance[Entrance::Left].push_back(fragment);
		if (names[i].find('E') != std::string::npos)
			mFragmentsWithEntrance[Entrance::Right].push_back(fragment);

		file.close();
	}
}

void Maze::PurgeFragments()
{
	for (int i = 0; i < mFragments.size(); ++i)
	{
		delete mFragments[i];
	}
	mFragments.clear();
}

sf::Vector2f Maze::GetStartingPos()
{
	return mStart;
}

int Maze::GetTileSize()
{
	return TileSize;
}

TileInfo * Maze::GetTile(const int & x, const int & y)
{
	return mMap[y][x];
}

void Maze::InitializeVector()
{
	mMap.resize(mMazeSize*FragmentHeight);
	for (int i = 0; i < mMap.size(); ++i)
	{
		mMap[i].resize(mMazeSize*FragmentWidth);
		for (int j = 0; j < mMap[i].size(); ++j)
		{
			mMap[i][j] = nullptr;
		}
	}
}

void Maze::SetupTileSet()
{
	TileInfo* tile;
	tile = new TileInfo;
	tile->mTexture = "FillingTile";
	mTextureManager->RequireResource(tile->mTexture);
	tile->mSprite.setTexture(*mTextureManager->GetResource(tile->mTexture));
	tile->mIsCollidable = true;
	mTileSet.push_back(tile);
	tile = new TileInfo;
	tile->mTexture = "WallTile";
	mTextureManager->RequireResource(tile->mTexture);
	tile->mSprite.setTexture(*mTextureManager->GetResource(tile->mTexture));
	tile->mIsCollidable = true;
	mTileSet.push_back(tile);
	tile = new TileInfo;
	tile->mTexture = "FloorTile";
	mTextureManager->RequireResource(tile->mTexture);
	tile->mSprite.setTexture(*mTextureManager->GetResource(tile->mTexture));
	tile->mIsCollidable = false;
	mTileSet.push_back(tile);
	tile = new TileInfo;
	tile->mTexture = "PortalTile";
	mTextureManager->RequireResource(tile->mTexture);
	tile->mSprite.setTexture(*mTextureManager->GetResource(tile->mTexture));
	tile->mIsCollidable = false;
	tile->mIsEnd = true;
	mTileSet.push_back(tile);
}

void Maze::PurgeTileSet()
{
	for (int i = 0; i < mTileSet.size(); ++i)
	{
		mTextureManager->ReleaseResource(mTileSet[i]->mTexture);
		delete mTileSet[i];
	}
	mTileSet.clear();
}

void Maze::GenerateMazeProper(int x, int y, const Entrance & lEntry)
{
	x = x / FragmentWidth * FragmentWidth;
	y = y / FragmentHeight * FragmentHeight;

	if (x < 0 || y < 0 || x >= mMazeSize * FragmentWidth || y >= mMazeSize * FragmentHeight)
		return;
	if (mMap[y][x] != nullptr)
		return;

	int random = rand() % 7;
	Fragment* chosenFragment = mFragmentsWithEntrance[lEntry][random];
	InsertFragment(x, y, chosenFragment);

	if (std::find(mFragmentsWithEntrance[Entrance::Left].begin(), mFragmentsWithEntrance[Entrance::Left].end(), chosenFragment) != mFragmentsWithEntrance[Entrance::Left].end())
	{
		GenerateMazeProper(x - FragmentWidth, y, Entrance::Right);
	}
	if (std::find(mFragmentsWithEntrance[Entrance::Right].begin(), mFragmentsWithEntrance[Entrance::Right].end(), chosenFragment) != mFragmentsWithEntrance[Entrance::Right].end())
	{
		GenerateMazeProper(x + FragmentWidth, y, Entrance::Left);
	}
	if (std::find(mFragmentsWithEntrance[Entrance::Up].begin(), mFragmentsWithEntrance[Entrance::Up].end(), chosenFragment) != mFragmentsWithEntrance[Entrance::Up].end())
	{
		GenerateMazeProper(x, y - FragmentHeight, Entrance::Down);
	}
	if (std::find(mFragmentsWithEntrance[Entrance::Down].begin(), mFragmentsWithEntrance[Entrance::Down].end(), chosenFragment) != mFragmentsWithEntrance[Entrance::Down].end())
	{
		GenerateMazeProper(x, y + FragmentHeight, Entrance::Up);
	}
}

void Maze::InsertFragment(const int& x, const int& y, Fragment * lFragment)
{
	for (int i = 0, k=y; i < FragmentHeight; ++i, ++k)
	{
		for (int j = 0, l=x; j < FragmentWidth; ++j, ++l)
		{
			mMap[k][l] = lFragment->mTile[i][j];
		}
	}
}

void Maze::FillBoundaries()
{
	TileInfo* fillingTile = mTileSet[0];
	for (int i = 0; i < mMap.size(); ++i)
	{
		if (i == 0 || i == mMap.size() - 1)
		{
			for (int j = 0; j < mMap[i].size(); ++j)
			{
				mMap[i][j] = fillingTile;//filling tile;
			}
		}
		else
		{
			mMap[i][0] = fillingTile;
			mMap[i][mMap[i].size() - 1] = fillingTile;
		}
	}
}

void Maze::AddWallsWhereNeeded()
{
	TileInfo* fillingTile = mTileSet[0];
	TileInfo* wallTile = mTileSet[1];
	TileInfo* floorTile = mTileSet[2];
	
	for (int i = 1; i < mMap.size(); ++i)
	{
		for (int j = 0; j < mMap[i].size(); ++j)
		{
			if (mMap[i][j] == floorTile && mMap[i - 1][j] == fillingTile)
			{
				mMap[i][j] = wallTile;
			}
		}
	}
}

void Maze::PlaceEndPortal(int x, int y)//za d³ugo dzia³a ;(
{
	std::queue<std::pair<int, int>> que;
	std::pair<int, int> coords;
	std::vector<std::vector<bool>> isVisited;

	isVisited.resize(mMap.size());
	for (int i = 0; i < isVisited.size(); ++i)
	{
		isVisited[i].resize(mMap[i].size());
		for (int j = 0; j < isVisited[i].size(); ++j)
		{
			isVisited[i][j] = false;
		}
	}

	que.push(std::make_pair(y, x));
	isVisited[coords.first][coords.second] = true;
	while (!que.empty())
	{
		coords = que.front();
		que.pop();
		if (mMap[coords.first + 1][coords.second] != nullptr)
		{
			if (mMap[coords.first + 1][coords.second]->mIsCollidable == false && isVisited[coords.first + 1][coords.second] == false)
			{
				que.push(std::make_pair(coords.first + 1, coords.second));
				isVisited[coords.first + 1][coords.second] = true;
			}
		}
		if (mMap[coords.first - 1][coords.second] != nullptr)
		{
			if (mMap[coords.first - 1][coords.second]->mIsCollidable == false && isVisited[coords.first - 1][coords.second] == false)
			{
				que.push(std::make_pair(coords.first - 1, coords.second));
				isVisited[coords.first - 1][coords.second] = true;
			}
		}
		if (mMap[coords.first][coords.second + 1] != nullptr)
		{
			if (mMap[coords.first][coords.second + 1]->mIsCollidable == false && isVisited[coords.first][coords.second + 1] == false)
			{
				que.push(std::make_pair(coords.first, coords.second + 1));
				isVisited[coords.first][coords.second + 1] = true;
			}
		}
		if (mMap[coords.first][coords.second - 1] != nullptr)
		{
			if (mMap[coords.first][coords.second - 1]->mIsCollidable == false && isVisited[coords.first][coords.second - 1] == false)
			{
				que.push(std::make_pair(coords.first, coords.second - 1));
				isVisited[coords.first][coords.second - 1] = true;
			}
		}
	}

	mMap[coords.first][coords.second] = mTileSet[3];
}
