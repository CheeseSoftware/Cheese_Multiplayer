#pragma once

#ifndef MAINMENU_H_INCLUDED
#define MAINMENU_H_INCLUDED

#include <functional>
#include <vector>
#include "MenuState.h"
#include "GameState.h"

class MainMenu : public MenuState
{
private:
    sf::Image Image;
    sf::Sprite *sprite;
	std::function<void()> lambda;
public:
	MainMenu();
    ~MainMenu();
    //virtual void Initialize();
    virtual void EventUpdate(App& app, sf::Event& event);
    virtual GameState *Update(App& app);
    virtual void Draw(App& app);
    //virtual void Shutdown();
};

#endif // MAINMENU_H_INCLUDED
