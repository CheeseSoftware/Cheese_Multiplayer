#pragma once

#ifndef MENUITEM_H_INCLUDED
#define MENUITEM_H_INCLUDED

#include <functional>
#include <SFML/Graphics.hpp>
#include "GameState.h"

class MenuItem
{
protected:
    int x;
    int y;
    int width;
    int height;
    bool selected;
    bool down;
	bool clicked;
    std::function<void()> clickEvent;
public:
    MenuItem(int x, int y, int width, int height, std::function<void()> &clickEvent);
	~MenuItem();
    virtual void EventUpdate(sf::Event& event, App& app);
    virtual GameState *Update(App& app);
    virtual void Draw(App& app)=0;
};

#endif // MENUITEM_H_INCLUDED
