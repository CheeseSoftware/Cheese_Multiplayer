#ifndef _SERVER
#include "camera.h"

Camera::Camera(double speed)
{
	speed = speed;
	currentEntity = nullptr;

	setSize(1152,720);// temporära magiska nummer
}


Camera::~Camera(void)
{

}

void Camera::Update(App& app)
{
	if (currentEntity != nullptr)
	{
		float deltaX = currentEntity->getX()+8-getCenter().x;
		float deltaY = currentEntity->getY()+8-getCenter().y;
		float speedFactor = atan(app.getFrameTime()*speed)*2/3.14159265358979323846264338327950288419;

		setCenter(getCenter().x + deltaX*speedFactor, getCenter().y + deltaY*speedFactor);

		rotate(13);
	}
}

void Camera::setCameraAt(Entity &entity)
{
	if (currentEntity != &entity)
		currentEntity = &entity;
}

void Camera::setSpeed(double speed)
{
	this->speed = speed;
}

sf::Vector2f Camera::getEntityPosition()
{
	if(currentEntity != nullptr)
		return(currentEntity->getPosition());
	else
		return(sf::Vector2f(0, 0));
}

Entity &Camera::getEntity() { return *currentEntity; }
#endif