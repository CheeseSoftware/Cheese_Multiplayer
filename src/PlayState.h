#pragma once
#include "gamestate.h"
#include "World.h"
#include "Creature.h"
#include "Player.h"
#include "Monster.h"
#include "TextureContainer.h"
#include "Camera.h"

class PlayState : public GameState
{
public:
	PlayState(sf::RenderWindow *window);
	~PlayState();
    virtual void EventUpdate(sf::Event &event);
    virtual GameState *Update(sf::RenderWindow &app);
    virtual void Draw(sf::RenderWindow &app);
	void AddCreature(Creature*);
	void AddPlayer(Player*);
	World* currentWorld;
	Player* player;
	sf::Image playerImage;
	sf::Sprite* playerSprite;
	sf::Sprite *spriteList;
	std::vector<Creature*> creatureList;
	std::vector<Player*> playerList;
	TextureContainer tc;
	Camera *camera;
};

