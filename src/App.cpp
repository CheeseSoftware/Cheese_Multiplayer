//#ifdef _SERVER
#include "App.h"

#define MIN_FRAME_TIME 0.00390625F

#ifndef _SERVER
App::App(sf::VideoMode videoMode) : tgui::Window(videoMode, "Cheese Multiplayer - Alpha")
{
	globalFont.loadFromFile("TGUI/Fonts/DejaVuSans.ttf");
}
#endif

float App::getFrameTime()
{
	//std::cout << frameTime << " " << sleptTime << std::endl;
	return (frameTime+sleptTime)/1000000;
}

void App::Update()
{
	frameTime = frameTimer.getElapsedTime().asMicroseconds();

	if (frameTime < MIN_FRAME_TIME)
	{
		sleptTime = MIN_FRAME_TIME-frameTime;
		sf::sleep(sf::milliseconds(sleptTime));
	}
	else
	{
		sleptTime = 0;
	}

	frameTimer.restart();
}

sf::Clock& App::getFrameTimer()
{
	return frameTimer;
}
//#endif