#pragma once
#include "ResourceManager.h"
#include <SFML/Graphics.hpp>

class FontManager : public ResourceManager<FontManager, sf::Font>
{
public:
	FontManager(): ResourceManager("fonts.cfg"){}

	sf::Font* Load(const std::string& lPath)
	{
		sf::Font* newFont = new sf::Font;
		if (!newFont->loadFromFile(lPath))
		{
			std::cout << "Error: Unable to open " << lPath << std::endl;
			delete newFont;
			return nullptr;
		}
		return newFont;
	}
};
