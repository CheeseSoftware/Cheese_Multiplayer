#include "player.h"
#include <string>
#include <list>
#include "Camera.h"
#include <SFML/Graphics.hpp>

/*class Player : public Creature
{
private:
    std::string name;
    bool leftIsDown = false;
    bool downIsDown = false;
    bool rightIsDown = false;
    bool upIsDown = false;
    public:

    virtual void Update(sf::RenderWindow &App);
    virtual void Draw(sf::RenderWindow &App);
};*/

Player::Player(float X, float Y, bool IsClientControlling, std::string spriteName, int spriteIndex, std::string Name) : Creature(X, Y, 4096,0.875, spriteName, spriteIndex, IsClientControlling)
{
    name = Name;
	cameraDelay = 0;
    right = false;
    down = false;
    left = false;
    up = false;
}

void Player::Update(sf::RenderWindow &app, sf::View &camera)
{
    if (isClientControlling)
    {
        KeyUpdate(
            app.GetInput().IsKeyDown(sf::Key::D),
            app.GetInput().IsKeyDown(sf::Key::S),
            app.GetInput().IsKeyDown(sf::Key::A),
            app.GetInput().IsKeyDown(sf::Key::W));

		if (cameraDelay <= 0)
		{
			reinterpret_cast<Camera&>(camera).setCameraAt(*this);
		}
		else
		{
			cameraDelay -= app.GetFrameTime();
		} 
	}

    Creature::Update(app, camera);
}

void Player::Draw(sf::RenderWindow &app, TextureContainer &tc)
{
    Creature::Draw(app, tc);
}

void Player::KeyUpdate(bool Right, bool Down, bool Left, bool Up)
{
    if (Right != right || Down != down || Left != left || Up != up)
    {
        horizontal = 0;
        vertical = 0;

        if (Right)  horizontal += speed;
        if (Down)   vertical += speed;
        if (Left)   horizontal -= speed;
        if (Up)     vertical -= speed;

        right = Right;
        down = Down;
        left = Left;
        up = Up;

        if (isClientControlling)
        {
            
        }
    }
}

std::string Player::getTextureName()
{
	return "blocksolid.png";
}

char Player::getTextureId()
{
	return 0;
}