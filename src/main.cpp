#include "App.h"

#ifndef _SERVER
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "PlayState.h"
#else
#include "ServerState.h"

#endif

int _argc;
char** _argv;
//using namespace sf;

/*#ifdef _SERVER
#define _APP *reinterpret_cast<tgui::Window*>(&app)//static_cast<tgui::Window>(app)
#else
#define _APP app
#endif*/

int main(int argc, char** argv)
{
	_argc = argc;
	_argv = argv;

#ifndef _SERVER

	App app(sf::VideoMode(1152,720));

	GameState *gameState = new PlayState((const PlayState&)app);
#else
	App app;
	GameState *gameState = new ServerState(app);
#endif

	GameState *oldState;

	//app.SetFramerateLimit(6);
#ifndef _SERVER
	while (app.isOpen())
	{
		sf::Event event;
		if (app.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				app.close();
			gameState->EventUpdate(app, event);
			// Pass the event to all the objects (if there would be objects)
            app.handleEvent(event);
		}
#else
	while (true)
	{
#endif

		oldState = gameState;
		if((gameState=gameState->Update(app)) != oldState)
		{
			delete oldState;
		}

#ifndef _SERVER
		app.clear();

		gameState->Draw(app);

		app.Update();
		app.display();
#else
		app.Update();
#endif
	}

	return 0;
}