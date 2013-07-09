#pragma once

#ifndef Label_H_INCLUDED
#define Label_H_INCLUDED

#include <SFML/Graphics.hpp>
#include "MenuItem.h"
#include "GameState.h"
#include "App.h"

class Label : public MenuItem
{
public:
	Label(int x, int y, int width, int height, std::function<void()> &clickEvent);
	~Label();
	virtual void EventUpdate(sf::Event& event, App& app);
    virtual GameState *Update(App& app);
    virtual void Draw(App& app);
};

#endif