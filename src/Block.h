#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "TextureContainer.h"

class Block
{
public:
	Block(void);
	unsigned char metadata;
	unsigned char getMetadata();
    virtual short int getId()=0;
	virtual char getTextureId()=0;
	virtual std::string getTextureName()=0;
	virtual bool isSeeThrough()=0;
	//virtual char getSubTextureId() = 0;
	void setMetadata(unsigned char);
	void Draw(short posX, short posY, short chunkPosX, short chunkPosY, sf::RenderWindow &app, TextureContainer &tc);
};

