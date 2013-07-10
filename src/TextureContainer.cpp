#include <iostream>
#include <SFML\Graphics.hpp>
#include "TextureContainer.h"


TextureContainer::TextureContainer(void)
{
	AddSpriteSheet("Block0.png", 16, 16);
	AddSpriteSheet("BlockBackground.png", 16, 16);
	AddSpriteSheet("BlockSolid.png", 16, 16);
	//AddSpriteSheet("BlockGravity.png", 16, 16);
	AddSpriteSheet("arrow.png", 32, 8);

	AddSpriteSheet("graywizard.png", 16, 26);
}


TextureContainer::~TextureContainer(void)
{

}

bool TextureContainer::AddSpriteSheet(std::string fileName, int spriteWidth, int spriteHeight)
{
	sf::Image image;
	bool success = image.loadFromFile(fileName);

	if (!success)
	{
		std::cout << "Failed to load " << fileName << '\n';
	}

	int width = image.getSize().x/spriteWidth;
	int height = image.getSize().y/spriteHeight;

	sf::Sprite *sprite = new sf::Sprite[width*height*height];
	sf::Image *tempImage;

	for (int y = 0; y < width; y++)
	{
		for (int x = 0; x < height; x++)
		{
			tempImage = new sf::Image();
			tempImage->create(spriteWidth, spriteHeight);
			tempImage->copy(image, 0, 0, sf::IntRect(x * spriteWidth, y * spriteHeight, (x + 1) * spriteWidth, (y + 1) * spriteHeight), false);
			//tempImage->setSmooth(false);
			sf::Texture *toSet = new sf::Texture();
			toSet->create(spriteWidth, spriteHeight);
			toSet->update(*tempImage, 0, 0);
			sprite[x + (y*width)].setTexture(*toSet);
		}
	}
	textureList.emplace(fileName, sprite);
	std::cout << "Added texturesheet " << fileName << std::endl;
	return success;
}

sf::Sprite *TextureContainer::getTextures(std::string textureName)
{
	auto it = textureList.find(textureName);
	return (it == textureList.end()) ? nullptr : it->second;
}