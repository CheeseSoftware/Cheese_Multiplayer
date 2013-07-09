#ifndef _SERVER

#include <SFML/Graphics.hpp>
#include <vector>
#include "MenuState.h"
#include "MenuItem.h"                            
#include "GameState.h"

MenuState::MenuState()//void MenuState::Initialize()
{

}

MenuState::~MenuState()//void MenuState::Shutdown()
{
    for (std::vector<double>::size_type i = 0; i < menuItems.size(); i++)
    {
        delete menuItems[i];
    }
}

void MenuState::EventUpdate(App& app, sf::Event& event)
{

}

GameState *MenuState::Update(App& app)
{
    return this;
}

void MenuState::Draw(App& app)
{
    for (unsigned int i = 0; i < menuItems.size(); i++)
    {
        menuItems[i]->Draw(app);
    }
}
#endif