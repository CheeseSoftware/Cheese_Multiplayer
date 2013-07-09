#include <SFML/Graphics.hpp>
#include "MenuItem.h"
#include "GameState.h"
#include "App.h"


MenuItem::MenuItem(int x, int y, int width, int height, std::function<void()> &clickEvent)
{
	this->x = x; this->y = y;
	this->width = width;
	this->height = height;
	this->clickEvent = clickEvent;
	selected = false;
	down = false;
	clicked = false;
}

MenuItem::~MenuItem()
{

}

void MenuItem::EventUpdate(sf::Event& event, App& app)
{
    if (event.type == sf::Event::MouseMoved)
    {
		selected = (sf::Mouse::getPosition().x >= x && sf::Mouse::getPosition().x <= x+width && sf::Mouse::getPosition().y >= y && sf::Mouse::getPosition().y <= y+height);
    }
    else if (event.type == sf::Event::MouseButtonPressed)
    {
		if (event.key.code != sf::Mouse::Left && selected)
        {
            down = true;
        }
    }
    else if (event.type == sf::Event::MouseButtonReleased)
    {
		if (event.key.code != sf::Mouse::Left)
        {
			clicked |= down;
			down = false;
        }
    }
}

GameState *MenuItem::Update(App& app)
{
	if (clicked && selected && clickEvent != 0)
			clickEvent();

	clicked = false;
	return 0;
}