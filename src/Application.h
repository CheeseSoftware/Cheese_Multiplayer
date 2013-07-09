#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
class Application : public sf::RenderWindow
{
public:
	Application(void);
	~Application(void);
	sf::Time getFrameTime();
	void setFrameTime(sf::Time);
private:
	sf::Time frameTime;
};

