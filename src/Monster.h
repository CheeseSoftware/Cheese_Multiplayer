#pragma once

#include "Creature.h"

class Monster : public Creature
{
public:
	~Monster(void);

#ifdef _SERVER
	Monster(float x, float y, short sizeX, short sizeY, float speed, float friction, std::string spriteName, int spriteIndex, bool isClientControlling);

	virtual void Update(App& app, World* world, std::queue<sf::Packet>* packetDataList, Camera* camera);
#else
	Monster(float x, float y, short sizeX, short sizeY, float speed, float friction, std::string spriteName, int spriteIndex, bool isClientControlling);

	virtual void Update(App& app, World* world, std::queue<sf::Packet>* packetDataList,Camera* camera, EventHandler& EventHandler);
#endif
};