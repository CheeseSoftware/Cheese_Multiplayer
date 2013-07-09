#include "Projectile.h"
#include "Entity.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "World.h"
#include "BlockSolid.h"

Projectile::Projectile(float x, float y, short sizeX, short sizeY, float angle, float speed, float friction, std::string spriteName, int spriteIndex, bool isClientControlling) 
	: Entity(x,y,sizeX,sizeY,angle,speed,friction,spriteName,spriteIndex,isClientControlling)
{
	speedX = (float)cos(angle * M_PI/180) * speed;
	speedY = (float)sin(-angle * M_PI/180) * speed;
}


Projectile::~Projectile(void)
{
}

void Projectile::Update(App& app, World* world, std::queue<sf::Packet>* packetDataList)
{ 
	speed = sqrt(pow(abs(speedX),2)+pow(abs(speedY),2));
	friction = 100/speed;
	if (friction > 1)
		friction = 1;

	Entity::Update(app, world, packetDataList);
}

void Projectile::Collision(World* world)
{
	//world.setBlock(2, (int)x<<4-16, (int)y<<4,1);
	speedX = 0;
	speedY = 0;
}

std::string Projectile::getTextureName() { return "arrow.png"; }
char Projectile::getTextureId() { return 0; }

/*void Projectile::Draw(App& app)
{
	app.Draw(*sprite);
}

void Projectile::Rotate(float degrees)
{
	sprite->Rotate(degrees);
}

void Projectile::setAngle(float angle)
{
	sprite->SetRotation(-angle);
}

void Projectile::setSpeed(float speed)
{
	if(speed >= 0)
	{
		this->speed = speed;
	}
}

void Projectile::setPosition(sf::Vector2f& position)
{
	sprite->SetPosition(position);
}

void Projectile::setTexture(sf::Sprite& sprite)
{
	this->sprite->SetImage(*(sprite.GetImage()));
}

void Projectile::setSize(sf::Vector2f& size)
{
	sprite->Resize(size);
}

float Projectile::getAngle() {return sprite->GetRotation();}
sf::Vector2f Projectile::getPosition() {return sprite->GetPosition();}
float Projectile::getSpeed() {return speed;}
sf::Vector2f Projectile::getSize() {return sprite->GetSize();}
*/