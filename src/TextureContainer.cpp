#include <iostream>
#include <SFML\Graphics.hpp>
#include "TextureContainer.h"


TextureContainer::TextureContainer(void)
{
	AddSpriteSheet("BlockBackground.png", 16, 16);
	AddSpriteSheet("BlockSolid.png", 16, 16);
	AddSpriteSheet("BlockGravity.png", 16, 16);

	AddSpriteSheet("graywizard.png", 16, 26);
}


TextureContainer::~TextureContainer(void)
{

}


/*bool TextureContainer::AddTexture(std::string fileName, int textureType)
{
	sf::Image *image = new sf::Image();
	bool success = image->LoadFromFile(fileName);
	image->SetSmooth(false);
	sf::Sprite *sprite = new sf::Sprite();
	sprite[0].SetImage(*image); 
	blockTextureList.
	return success;
}*/

bool TextureContainer::AddSpriteSheet(std::string fileName, int spriteWidth, int spriteHeight)
{
	sf::Image image;
	bool success = image.LoadFromFile(fileName);

	int width = image.GetWidth()/spriteWidth;
	int height = image.GetHeight()/spriteHeight;

	sf::Sprite *sprite = new sf::Sprite[width*height];
	sf::Image *tempImage;

	for (int x = 0; x+1 <= width; x++)
	{
		for (int y = 0; y+1 <= height; y++)
		{
			tempImage = new sf::Image(spriteWidth, spriteHeight);
			tempImage->Copy(image, 0, 0, sf::IntRect(x*spriteWidth, y*spriteHeight, spriteWidth, spriteHeight), false);
			tempImage->SetSmooth(false);
			sprite[x+y*width].SetImage(*tempImage);
		}
	}
	//textureList[textureType]->push_back(sprite);
	textureList.emplace(fileName, sprite);
	std::cout << "Added texturesheet " << fileName << std::endl;
	return success;
}

sf::Sprite *TextureContainer::getTextures(std::string textureName)
{
	auto it = textureList.find(textureName);
	return (it == textureList.end()) ? NULL : it->second;
}