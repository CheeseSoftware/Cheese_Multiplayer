#include "BlockSolid.h"

BlockSolid::BlockSolid(unsigned short id) : Block(id)
{

}

std::string BlockSolid::getTextureName()
{
	return "BlockSolid.png";
}

unsigned short BlockSolid::getTextureId(App &app, unsigned short metadata)
{
	return metadata;
}

unsigned short BlockSolid::getId()
{
	return 1;
}

bool BlockSolid::isSeeThrough()
{
	return(false);
}

bool BlockSolid::isSolid()
{
	return(true);
}

unsigned char BlockSolid::getLayer()
{
	return 2;
}
