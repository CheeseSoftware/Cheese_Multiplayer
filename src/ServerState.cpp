#include <iostream>
#include <string>
#include <SFML\Network.hpp>
#include "ServerState.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "World.h"
#include "BlockSolid.h"
#include "BlockBackground.h"
#include "App.h"
#include <SFML\Graphics.hpp>



ServerState::ServerState(App& app)
{
	currentWorld = new World();
	sC = new ServerConnection(5001, currentWorld);
	//sC->Launch();
	//unsigned short i = 1;

	//currentWorld->RegisterBlock(i,(new BlockSolid(i))->RegisterBlock(i++));
	//currentWorld->RegisterBlock(i,(new BlockBackground(i))->RegisterBlock(i++));
}

ServerState::~ServerState()
{
	delete currentWorld;
}

GameState *ServerState::Update(App& app)
{
	//std::cout << "updates per second: " << 1/APP(app).getFrameTime() << std::endl;
	std::queue<sf::Packet>* packetDataList = currentWorld->Update(app, tC, nullptr);
	while (!packetDataList->empty())
	{
		sC->Broadcast(packetDataList->front());
		packetDataList->pop();
	}
	//delete packetDataList;
	sC->Run();
	ProcessPackets();
	return this;
}

void ServerState::ProcessPackets(void)
{
	//sC->globalMutex.lock();
	auto packets = sC->packets;
	sC->packets = std::queue<std::pair<sf::Packet*, Client*>>();
	//sC->globalMutex.unlock();

	while(packets.size() > 0)
	{
		auto data = packets.front();
		sf::Packet* packet = data.first;
		sf::Packet* originalPacket = new sf::Packet(*packet);
		Client* client = data.second;
		//Now process packets
		sf::Uint16 packetType;
		if(!(*packet >> packetType))
			std::cout << "ERROR: Server could not extract data" << std::endl;

		switch(packetType)
		{
		case PingMessage: //measure ping between sent 1 and received 1 (type)
			{
				float ping = client->pingClock.getElapsedTime().asMilliseconds();
				client->pingClock.restart();
				client->ping = ping;
				//std::cout << "Client " << client->ID << " has ping " << ping << std::endl;
			}
			break;
		case KickMessage: //server kicks client (type, string message)

			break;
		case PlayerJoinLeft:
			{
				sf::Uint16 type;
				float xPos;
				float yPos;

				//std::cout << "Server got PlayerJoinLeft " << packetType << " " << type << " " << xPos << " " << yPos << " " << client->ID << std::endl;

				sf::Packet send;
				sf::Uint16 packetTypeTemp = PlayerJoinLeft;
				sf::Uint16 clientidtemp = client->ID;

				*packet >> type;

				if(type == 0) //Player has joined
				{
					*packet >> xPos >> yPos;

					//Add the player to the server world
#ifdef _SERVER
					currentWorld->AddPlayer(client->ID, new Player(xPos, yPos, 16, 16, true, "graywizard.png", 0, "temp"));
#else
					//orkar inte fixaA>.<
#endif

					// Send the init message
					// Players
					send << (sf::Uint16) InitMessage;
					for(std::pair<int, Client*> pair : sC->clients)
					{
						Player* temp = currentWorld->GetPlayer(pair.first);
						if(temp != nullptr)
							send << (sf::Int16)pair.first << (sf::Int16)temp->getPosition().x << (sf::Int16)temp->getPosition().y << (sf::Int16)temp->getSize().x << (sf::Int16)temp->getSize().y;
					}
					client->socket.send(send);

					send.clear();
					send << packetType << type << xPos << yPos << clientidtemp;

				}
				/*else if(type == 1) //Player has left
				{
					currentWorld->RemovePlayer(client->ID);
					send.Clear();
					send << packetType << type << (sf::Uint16)client->ID;
					std::cout << client->IP << " has left" << std::endl;
				}*/

				sC->Broadcast(send);
				//std::cout << "Server sent PlayerJoinLeft " << packetType << " " << type << " " << xPos << " " << yPos << " " << clientidtemp << std::endl;
				break;
			}
		case PlayerMove:
			{
				float xPos;
				float yPos;
				float speedX;
				float speedY;
				float angle;
				float horizontal;
				float vertical;
				*packet >> xPos >> yPos >> speedX >> speedY >> angle >> horizontal >> vertical;
				Player* p = currentWorld->GetPlayer(client->ID);
				if (p != nullptr)
				{
					//Broadcast playermove data
					sf::Packet packet;
					sf::Int16 clientid = client->ID;
					packet << (sf::Uint16)PlayerMove << clientid << xPos << yPos << speedX << speedY << angle << horizontal << vertical;
					sC->Broadcast(packet);

					//Move player in server world
					p->CreatureMove(xPos, yPos, speedX, speedY, angle, horizontal, vertical);

					//Send world data in radius around player
					/*int chunkX = xPos * 0.00390625;
					int chunkY = yPos * 0.00390625;
					currentWorld->get*/
				}
			}
			break;
		case BlockPlace:
			{
				sf::Int32 xPos;
				sf::Int32 yPos;
				sf::Uint16 layer;
				sf::Uint16 id;
				sf::Uint16 metadata;
				*packet >> xPos >> yPos >> layer >> id >> metadata;
				currentWorld->setBlockAndMetadata(xPos, yPos, layer, id, metadata);
			}
			break;
		case BlockMetadataChange:
			{
				sf::Int32 xPos;
				sf::Int32 yPos;
				sf::Uint16 layer;
				sf::Uint16 metadata;
				*packet >> xPos >> yPos >> layer >> metadata;
				currentWorld->setBlockMetadata(xPos, yPos, layer, metadata);
			}
			break;
		}
		delete packet;
		delete originalPacket;
		packets.pop();
	}
}