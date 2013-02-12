#include "Camera.h"

Camera::Camera(double speed)
{
	this->speed = speed;
	currentCreature = NULL;
}


Camera::~Camera(void)
{

}

void Camera::Update(sf::RenderWindow &app)
{
	if (currentCreature != NULL)
		this->SetCenter(
		(currentCreature->getX()+8)*speed*app.GetFrameTime()+
		this->GetCenter().x*(1-speed*app.GetFrameTime()),
		(currentCreature->getY()+8)*speed*app.GetFrameTime()+
		this->GetCenter().y*(1-speed*app.GetFrameTime()));
}

void Camera::setCameraAt(Creature &creature)
{
	if (currentCreature != &creature)
		currentCreature = &creature;
}

void Camera::setSpeed(double speed)
{
	this->speed = speed;
}