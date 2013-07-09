#pragma once

#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "GameState.h"
#include "MenuState.h"

class Menu : public GameState
{
private:
    GameState *menuState;
public:
	Menu();
	~Menu();
    //virtual void Initialize();
    virtual void EventUpdate(App& app, sf::Event& event);
    virtual GameState *Update(App& app);
    virtual void Draw(App& app);
    //virtual void Shutdown();
};

#endif // MENU_H_INCLUDED
