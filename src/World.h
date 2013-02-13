#pragma once
#include "Chunk.h"
#include "Player.h"
#include "TextureContainer.h"
#include <iostream>
#include <map>
#include <string>
#include <functional>
#include <typeinfo>
#define SCREENSIZEX 800
#define SCREENSIZEY 600

class World
{
private:
	short int sizeX;
	short int sizeY;
	Chunk *chunkList[256][256];
	int*  playerList;
	std::map<unsigned short, std::function<Block*(unsigned short)>> blockTypeMap;
public:
	World(unsigned short, unsigned short);
	~World(void);
	void Draw(sf::RenderWindow &app, TextureContainer &tc, Player &player);
	void setBlock(short x, short y, Block* block);
	void AddBlockType(unsigned short, std::function<Block*(unsigned short)>);
	Block* getBlock(short x, short y);
	std::function<Block*(unsigned short)>* getBlockType(unsigned short id);
};

