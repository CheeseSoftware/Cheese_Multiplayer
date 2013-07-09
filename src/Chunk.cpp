#include "Chunk.h"
#include "Block.h"
#include "TextureContainer.h"
#include "camera.h"
#include "App.h"

#ifndef _SERVER
#include <SFML\Graphics.hpp>
#endif

Chunk::Chunk()
{ 
	for(int l = 0; l < 6; l++)
	{
		for (int x = 0; x < CHUNKWIDTH; x++)
		{
			for (int y = 0; y < CHUNKHEIGHT; y++)
			{
				blockList[x][y][l].first = nullptr;
				//blockList[x][y][l] = new BlockSolid(2);
			}
		}
	}
}			

Chunk::~Chunk(void)
{
	for(int l = 0; l < 6; l++)
	{
		for (int i = 0; i < CHUNKWIDTH; i++)
		{
			for (int j = 0; j < CHUNKHEIGHT; j++)
			{
				delete this->blockList[i][j][l].first;
			}
		}
	}
}

bool Chunk::isAnySolid(std::pair<Block*, unsigned short> blocks[6])
{
	for(int l = 0; l < 6; l++)
	{
		if(blocks[l].first != nullptr && blocks[l].first->isSolid())
			return true;
	}
	return false;
}

bool Chunk::isAnySeeThrough(std::pair<Block*, unsigned short> blocks[6])
{
	for(int l = 0; l < 6; l++)
	{
		if(blocks[l].first != nullptr && blocks[l].first->isSeeThrough())
			return true;
	}
	return false;
}

Block* Chunk::getHighestBlock(unsigned char x, unsigned char y)
{
	for(int l = 5; l >= 0; l--)
	{
		if(blockList[x][y][l].first != nullptr)
			return blockList[x][y][l].first;
	}
	return(0);
}

#ifndef _SERVER
void Chunk::Draw(long xPos, long yPos, App& app, TextureContainer &tc)
{
	//backgroundSprite = &(tc.getTextures("Block0.png")[0]);
	for(long x = 0; x < CHUNKWIDTH; x++)
	{
		for(long y = 0; y < CHUNKHEIGHT; y++)
		{
			long xPosBlock = (16*xPos-16 + x)*16;
			long yPosBlock = (16*yPos-16 + y)*16;
			if(blockList[x][y][0].first == nullptr || isAnySeeThrough(blockList[x][y]))
			{
				//backgroundSprite->SetPosition((16*xPos-16 + x)*16, (16*yPos-16 + y)*16);
				//app.Draw(*backgroundSprite);
			}
			for(int l = 5; l >= 0; l--)
			{
				if(blockList[x][y][l].first != nullptr && xPosBlock + 16 >= (GetCamera(app).getCenter().x - 381) && xPosBlock <= (GetCamera(app).getCenter().x + 381)&& yPosBlock + 16 >= (GetCamera(app).getCenter().y - 256) && yPosBlock <= (GetCamera(app).getCenter().y + 256))
				{
					blockList[x][y][l].first->Draw((xPos - 1 << 4) + x << 4, (yPos - 1 << 4) + y << 4, app, tc, blockList[x][y][l].second); 
					if(!blockList[x][y][l].first->isSeeThrough())
						break;
				}
			}
		}
	}
}
#endif

void Chunk::setBlock(unsigned char layer, unsigned short x, unsigned short y, Block *block)
{
	if (x >= 16 || y >= 16)
		return;

	blockList[x][y][layer].first = block;
}

void Chunk::setMetadata(unsigned char layer, unsigned short x, unsigned short y, unsigned short metadata)
{
	blockList[x][y][layer].second = metadata;
}

Block* Chunk::getBlock(unsigned char layer, unsigned short x, unsigned short y)
{
	return blockList[x][y][layer].first;
}
short Chunk::getMetadata(unsigned char layer, unsigned short x, unsigned short y) { return blockList[x][y][layer].second; }
unsigned short Chunk::getBlockId(unsigned char layer, unsigned short x, unsigned short y) { return blockList[x][y][layer].first->getId(); }