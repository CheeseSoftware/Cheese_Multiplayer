#include <SFML\Graphics.hpp>
#include "Creature.h"
#include "Monster.h"

Monster::Monster(float x, float y, short sizeX, short sizeY, float speed, float friction, std::string spriteName, int spriteIndex, bool isClientControlling)
	: Creature(x, y, sizeX, sizeY, speed, friction, spriteName, spriteIndex, isClientControlling)
{

}


Monster::~Monster(void)
{
}

#ifdef _SERVER
void Monster::Update(App& app, World* world, std::queue<sf::Packet>* packetDataList, Camera* camera)
#else
void Monster::Update(App& app, World* world, std::queue<sf::Packet>* packetDataList, Camera* camera, EventHandler& eventHandler)
#endif
{
#ifdef _SERVER
	Creature::Update(app, world, packetDataList, camera);
#else
	Creature::Update(app, world, packetDataList, camera, eventHandler);
#endif
}