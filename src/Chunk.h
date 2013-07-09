class Chunk;
#pragma once
#define CHUNKWIDTH 16
#define CHUNKHEIGHT 16
//#include "Block.h"
//#include "BlockSolid.h"
//#include "BlockBackground.h"
//#include "BlockGravity.h"
//#include "Player.h"
//#include "TextureContainer.h"
//#include <SFML/Graphics.hpp>
#include <stack>
#include "App.h"

class TextureContainer;
class Camera;
class Block;

class Chunk
{
private:
	std::pair<Block*, unsigned short> blockList[CHUNKWIDTH][CHUNKHEIGHT][6];
	bool drawBackground;
	//sf::Sprite *backgroundSprite;
public:
	Chunk();
	~Chunk(void);
	Block* getHighestBlock(unsigned char x, unsigned char y);
	Block* getBlock(unsigned char layer, unsigned short x, unsigned short y);
	short getMetadata(unsigned char layer, unsigned short x, unsigned short y);
	void setBlock(unsigned char layer, unsigned short x, unsigned short y, Block*);
	void setMetadata(unsigned char layer, unsigned short x, unsigned short y, unsigned short metadata);
	unsigned short getBlockId(unsigned char layer, unsigned short x, unsigned short y);
#ifndef _SERVER
	void Draw(long xPos, long yPos, App& app, TextureContainer &tc);
#endif
	bool isAnySolid(std::pair<Block*, unsigned short> blocks[6]);
	bool isAnySeeThrough(std::pair<Block*, unsigned short> blocks[6]);
};

