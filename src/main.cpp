#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "GameState.h"
#include "Menu.h"
#include "PlayState.h"
using namespace sf;

int main()
{
    sf::RenderWindow app(sf::VideoMode(768, 512), "SFML window");

	GameState *gameState = new PlayState(&app);

	//app.SetFramerateLimit(6);

	while (app.IsOpened())
    {
        sf::Event event;
       if (app.GetEvent(event))
        {
            if (event.Type == sf::Event::Closed)
                app.Close();

            gameState->EventUpdate(event);
        }

        GameState *oldState = gameState;
        if((gameState=gameState->Update(app)) != oldState)
        {
            delete oldState;
        }

        app.Clear();

        gameState->Draw(app);

        app.Display();
    }

    return EXIT_SUCCESS;
}

void Update()
{

}
