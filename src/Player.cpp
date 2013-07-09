#include "player.h"
#include "World.h"
#include "camera.h"
#include "Projectile.h"
#include <tuple>
#include "MessageType.h"
#include "App.h"

Player::Player(float X, float Y, short sizeX, short sizeY, bool IsClientControlling, std::string spriteName, int spriteIndex, std::string Name) 
	: Creature(X, Y, sizeX, sizeY, 4096, 0.875, spriteName, spriteIndex, IsClientControlling)
{
	name = Name;
	cameraDelay = 0;
	right = true;
	down = false;
	left = false;
	up = false;
	lmb = false;
}

void Player::Update(App& app, World* world, std::queue<sf::Packet>* packetDataList)
{
#ifndef _SERVER
	if (isClientControlling)
	{
		sf::Event event;
		app.pollEvent(event);



		if (&GetCamera(app).getEntity() != this)
		{
			if (cameraDelay <= 0)
			{
				GetCamera(app).setCameraAt(*this);
				cameraDelay = 0.5;
			}
			else
			{
				cameraDelay -= app.getFrameTime();
			}
		}
	}
#endif

	Creature::Update(app, world, packetDataList);
}

#ifndef _SERVER

void Player::EventUpdate(App& app, sf::Event& event, World* world, std::queue<sf::Packet>* packetDataList)
{
	if (isClientControlling)
	{
		bool rDown = false, dDown = false, lDown = false, uDown = false;
		switch(event.type)
		{
		case sf::Event::KeyPressed:
			switch(event.key.code)
			{
			case sf::Keyboard::D:
Right:
				rDown = true;
				break;
			case sf::Keyboard::S:
Down:
				dDown = true;
				break;
			case sf::Keyboard::A:
Left:
				lDown = true;
				break;
			case sf::Keyboard::W:
Up:
				uDown = true;
				break;

			case sf::Keyboard::Right:
				goto Right;

			case sf::Keyboard::Down:
				goto Down;

			case sf::Keyboard::Left:
				goto Left;

			case sf::Keyboard::Up:
				goto Up;
			}
			break;

		case sf::Event::MouseButtonPressed:
			switch(event.key.code)
			{
			case sf::Mouse::Left:
				if(!lmb && (lmb=true))
				{
					//double angle = atan2((GetCamera(app).GetCenter().y + app.GetInput().GetMouseY() - 256) - (GetCamera(app).getEntityPosition().y+8), (GetCamera(app).GetCenter().x + app.GetInput().GetMouseX() - 384) - (GetCamera(app).getEntityPosition().x+8)) * 180 / 3.1415;
					double angle = atan2((GetCamera(app).getCenter().y + sf::Mouse::getPosition().y - 256) - (y+8), (GetCamera(app).getCenter().x + sf::Mouse::getPosition().x - 384) - (x+8)) * 180 / 3.1415;
					if (angle < 0)
						angle = angle + 360;

					double deltaSpeedX = cos(angle*3.1415926535)*speedX;
					double deltaSpeedY = sin(angle*3.1415926535)*speedY;

					if (angle > 180)
						deltaSpeedX *= -1;

					if (angle < 90 || angle > 270)
						deltaSpeedY *= -1;


					//Projectile *projectile = new Projectile(GetCamera(app).getEntityPosition().x.getEntityPosition().y, 32, 32, -angle, 512, 0, "arrow.png", 0, false);
					Projectile *projectile = new Projectile(x+8, y+8, 32, 32, -angle, 2048, 0.03125, "arrow.png", 0, false);
					world->AddEntity(projectile);//new Projectile(sf::Vector2f(GetCamera(app).getCreaturePosition().x+8.getCreaturePosition().y+8), (float)angle, 500, tc.getTextures("arroaaawb.png")[0]));
					cameraDelay = 0.03125F;
					//GetCamera(app).setCameraAt(*projectile);
				}

				lmb = true;
				break;

			case sf::Mouse::Right:
				break;
			}

		case sf::Event::MouseButtonReleased:
			{
				lmb = false;
			}
			break;
		}
		KeyUpdate(rDown, dDown, lDown, uDown, packetDataList);
	}
}

void Player::Draw(App& app, TextureContainer &tc)
{
	Creature::Draw(app, tc);
}
#endif

void Player::KeyUpdate(bool Right, bool Down, bool Left, bool Up, std::queue<sf::Packet>* packetDataList)
{
	if (Right != right || Down != down || Left != left || Up != up)
	{
		horizontal = 0;
		vertical = 0;

		if (Right)  horizontal += speed;
		if (Down)   vertical += speed;
		if (Left)   horizontal -= speed;
		if (Up)     vertical -= speed;

		//std::cout << "bools: " <<  Right << " " << Down<< " " << Left<< " " << Up<< " " << std::endl;
		//std::cout << "variables: " <<  horizontal << " " << vertical << " " << speedX << " " << speedY << std::endl;

		right = Right;
		down = Down;
		left = Left;
		up = Up;



		if (isClientControlling)
		{
			sf::Packet packet;
			packet << (sf::Uint16)PlayerMove << x << y << speedX << speedY << angle << horizontal << vertical;
			packetDataList->push(packet);
		}
	}
}

void Player::setCameraDelay(float delay)
{
	cameraDelay = delay;
}

std::string Player::getTextureName()
{
	return "graywizard.png";
}

char Player::getTextureId()
{
	return 0;
}