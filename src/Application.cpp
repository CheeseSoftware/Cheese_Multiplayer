#include "Application.h"


Application::Application(void)
{
	
}


Application::~Application(void)
{
}

void Application::setFrameTime(sf::Time time)
{
	frameTime = time;
}

sf::Time Application::getFrameTime()
{
	return(frameTime);
}
