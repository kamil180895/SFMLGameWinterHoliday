#pragma once
#include <string>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <sstream>

template<class Derived, class T>
class ResourceManager
{
public:
	ResourceManager(const std::string& lPathsFile)
	{
		LoadPaths(lPathsFile);
	}
	virtual ~ResourceManager()
	{
		PurgeResources();
	}

	std::string GetPath(const std::string& lId)
	{
		auto path = mPaths.find(lId);
		if(path == mPaths.end())
		{
			return "";
		}
		else
		{
			return path->second;
		}
	}

	T* GetResource(const std::string& lId)
	{
		auto res = Find(lId);
		if (res)
		{
			return res->first;
		}
		else
		{
			return nullptr;
		}
	}

	bool RequireResource(const std::string& lId)
	{
		auto itr = mResources.find(lId);
		if (itr != mResources.end())
		{
			++(itr->second.second);
			return true;
		}

		auto path = mPaths.find(lId);
		if (path == mPaths.end()) { return false; }
		T* resource = Load(path->second);
		if (!resource) { return false; }
		mResources.emplace(lId, std::make_pair(resource, 1));
		return true;
	}

	bool ReleaseResource(const std::string& lId)
	{
		auto res = Find(lId);
		if (res == nullptr) { return false; }
		--(res->second);
		if (!res->second) { Unload(lId); }
		return true;
	}
private:
	std::pair<T*, unsigned int>* Find(const std::string& lId)
	{
		auto itr = mResources.find(lId);
		if (itr == mResources.end())
		{
			return nullptr;
		}
		else
		{
			return &itr->second;
		}
	}

	T* Load(const std::string& lPath)
	{
		return static_cast<Derived*>(this)->Load(lPath);
	}

	bool Unload(const std::string& lId)
	{
		auto itr = mResources.find(lId);
		if (itr == mResources.end())
			return false;
		delete itr->second.first;
		mResources.erase(itr);
		return true;
	}

	void PurgeResources()
	{
		while (mResources.begin() != mResources.end())
		{
			delete mResources.begin()->second.first;
			mResources.erase(mResources.begin());
		}
	}

	void LoadPaths(const std::string& lPathFile)
	{
		std::ifstream file;
		file.open(lPathFile);
		if (!file.is_open())
		{
			std::cout << "Error: Unable to open " << lPathFile << std::endl;
			return;
		}

		std::string line;

		while (std::getline(file, line))
		{
			std::stringstream keystream(line);
			std::string name;
			std::string path;
			keystream >> name;
			keystream >> path;
			mPaths.emplace(name, path);
		}
		file.close();
		return;
	}

	std::unordered_map<std::string, std::pair<T*, unsigned int>> mResources;
	std::unordered_map<std::string, std::string> mPaths;
};