#pragma once

class GameState;

#include <SFML\Graphics.hpp>
#include "App.h"

#ifndef GAMESTATE_H_INCLUDED
#define GAMESTATE_H_INCLUDED

//#include <SFML/Graphics.hpp>

class GameState {
public:
    //virtual void Initialize()=0;
    virtual GameState *Update(App& app)=0;
#ifndef _SERVER
	virtual void EventUpdate(App& app, sf::Event& event)=0;
    virtual void Draw(App& app)=0;
#endif
    //virtual void Shutdown()=0;
};

#endif // GAMESTATE_H_INCLUDED
