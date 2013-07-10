#include <iostream>
#include <typeinfo>
#include "Block.h"
#include "TextureContainer.h"

Block::Block(unsigned short id)
{
	this->id = id;
}

std::function<Block*(unsigned short)> Block::RegisterBlock(const unsigned short id)
{
	std::cout << typeid(*this).name() << '(' << typeid(*this).hash_code() << ") registered with blockId " << id << ".\n";
	return [&](unsigned short metadata) { return this; };
}

unsigned short Block::getId()
{
	return id;
}

bool Block::isSimple()
{
	return true;
}

#ifndef _SERVER
void Block::Draw(long posX, long posY, App& app, TextureContainer &tc, unsigned short metadata)
{
	sf::Sprite *&&tempSprite = &(tc.getTextures(getTextureName())[getTextureId(app, metadata)]);
	if (tempSprite != nullptr)
	{
		tempSprite->setPosition(posX, posY);
		app.draw(*tempSprite);
	}
}
#endif