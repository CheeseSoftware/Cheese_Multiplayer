#pragma once

#ifndef MENUSTATE_H_INCLUDED
#define MENUSTATE_H_INCLUDED

#include <vector>
#include "MenuItem.h"
#include "GameState.h"

class MenuState : public GameState
{
protected:
    std::vector<MenuItem*> menuItems;
public:
	MenuState();
	~MenuState();
    //virtual void Initialize();
    virtual void EventUpdate(App& app, sf::Event& event);
    virtual GameState *Update(App& app);
    virtual void Draw(App& app);
    //virtual void Shutdown();
};

#endif // MENUSTATE_H_INCLUDED
