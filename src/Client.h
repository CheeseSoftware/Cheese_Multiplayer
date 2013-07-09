#pragma once

#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include "MessageType.h"

struct Client
{
public:
	//Client(void);
	//~Client(void);
	sf::TcpSocket socket;
	sf::Uint16 ID;
	std::string name;
	sf::Uint16 ping;
	sf::Clock pingClock;
};

