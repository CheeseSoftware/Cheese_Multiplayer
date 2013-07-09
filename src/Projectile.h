class Projectile;
#pragma once

#include <SFML\Graphics.hpp>
#include <iostream>
#include "Entity.h"

class Camera;
class World;

class Projectile : public Entity
{
public:
	Projectile(float x, float y, short sizeX, short sizeY, float angle, float speed, float friction, std::string spriteName, int spriteIndex, bool isClientControlling);
	~Projectile(void);
	virtual void Update(App& app, World* world, std::queue<sf::Packet>* packetDataList);
	virtual void Collision(World* world);
	std::string getTextureName();
	char getTextureId();
	/*virtual void Draw(App& app);
	void Rotate(float degrees);
	void setAngle(float angle);
	float getAngle();
	void setSpeed(float speed);
	float getSpeed();
	void setSize(sf::Vector2f& size);
	sf::Vector2f getSize();
	void setPosition(sf::Vector2f& position);
	sf::Vector2f getPosition();
	void setTexture(sf::Sprite& sprite);
	sf::Sprite* sprite;
private:
	float speed;*/
};

