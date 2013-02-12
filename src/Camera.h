#pragma once
#include <SFML/Graphics.hpp>
#include "Creature.h"

class Camera : public sf::View
{
private:
	Creature *currentCreature;
	double speed;
public:
	Camera(double speed);
	~Camera(void);
	void Update(sf::RenderWindow &app);
	void setCameraAt(Creature &creature);
	void setSpeed(double speed);
};

