#pragma once

#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <thread>
#include <queue>
#include "Player.h"
#include "World.h"

#include "Client.h"

class Connection
{
public:
	Connection(int port, sf::IpAddress IP);
	~Connection(void);
	std::queue<sf::Packet*> packets;
	sf::Mutex globalMutex;
	Client* client;
	void Run();
private:
	//sf::Thread* thread;
	bool Connect(sf::IpAddress ip, int port);
	void Receive();
};

