#pragma once
#include "ResourceManager.h"
#include <SFML/Graphics.hpp>

class TextureManager : public ResourceManager<TextureManager, sf::Texture>
{
public:
	TextureManager(): ResourceManager("textures.cfg"){}

	sf::Texture* Load(const std::string& lPath)
	{
		sf::Texture* newTexture = new sf::Texture;
		if (!newTexture->loadFromFile(lPath))
		{
			std::cout << "Error: Unable to open" << lPath << std::endl;
			delete newTexture;
			return nullptr;
		}
		return newTexture;
	}
};