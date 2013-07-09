#include "Creature.h"

#define PI 3.141592653589793238462643383279502884197169399375105820974944

Creature::Creature(float x, float y, short sizeX, short sizeY, float speed, float friction, std::string spriteName, int spriteIndex, bool isClientControlling) 
	: Entity(x,y,sizeX,sizeY,0,speed,friction,spriteName,spriteIndex,isClientControlling)
{
	horizontal = 0;
    vertical = 0;
}

void Creature::Update(App& app, World* world, std::queue<sf::Packet>* packetDataList)
{
    speedX += horizontal * app.getFrameTime();
    speedY += vertical * app.getFrameTime();

	Entity::Update(app, world, packetDataList);
}

void Creature::CreatureMove(float x, float y, float speedX, float speedY, float angle, float horizontal, float vertical)
{
	this->x = x;
	this->y = y;
	this->speedX = speedX;
	this->speedY = speedY;
	this->angle = angle;
	this->horizontal = horizontal;
	this->vertical = vertical;
}