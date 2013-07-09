#pragma once
#include "gamestate.h"
#include "TextureContainer.h"
#include "ServerConnection.h"
#include "Player.h"
#include "App.h"

class World;
class InGameUI;
class Camera;

class ServerState : public GameState
{
	World* currentWorld;
	TextureContainer tC;
	ServerConnection* sC;
	void ProcessPackets();
public:
	ServerState(App& app);
	~ServerState();
	virtual void EventUpdate(App& app, sf::Event& event) {}
    virtual GameState *Update(App& app);
	virtual void Draw(App& app) {}
};
