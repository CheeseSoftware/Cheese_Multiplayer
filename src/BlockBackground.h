#pragma once
#include "Block.h"

class BlockBackground : public Block
{
public:
	BlockBackground(unsigned short id);
	virtual unsigned short getId();
	virtual unsigned short getTextureId(App &app, unsigned short metadata);
	virtual unsigned char getLayer();
	virtual std::string getTextureName();
	virtual bool isSeeThrough();
	virtual bool isSolid();
};

