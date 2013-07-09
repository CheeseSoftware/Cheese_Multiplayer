#include <SFML\Graphics.hpp>
#include "Creature.h"
#include "Monster.h"


Monster::Monster(float x, float y, short sizeX, short sizeY, float speed, float friction, std::string spriteName, int spriteIndex, bool isClientControlling) : Creature(x, y, sizeX, sizeY, speed, friction, spriteName, spriteIndex, isClientControlling)
{
}


Monster::~Monster(void)
{
}

void Monster::Update(App& app, World* world, std::queue<sf::Packet>* packetDataList)
{
	Creature::Update(app, world, packetDataList);
}