#ifndef _SERVER

#include <iostream>
#include <string>
#include <SFML\Network.hpp>
//#include <SFML\Graphics.hpp>
#include "GameState.h"
#include "PlayState.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "World.h"
#include "Camera.h"
#include "InGameUI.h"
#include "Player.h"
#include "BlockSolid.h"
#include "BlockBackground.h"
#include "EventHandler.h"

namespace sf
{
	class RenderWindow;
}

extern int _argc;
extern char** _argv;

PlayState::PlayState(App& app)
{
	char* str_ip = "127.0.0.1";

	if (_argc >= 2)
	{
		str_ip = _argv[1];
	}
	std::cout << "Connectiong to " << str_ip << " ...\n";

	sf::IpAddress ip(str_ip);//std::string ip;
	int port;



	camera = new Camera(32);
	currentWorld = new World();
	blockMenu = new InGameUI(app, tC, *currentWorld);
	connection = new Connection(5001, ip);

	app.setView(*reinterpret_cast<sf::View*>(camera));
	//camera->setSize(sf::Vector2f(768, 512)); 

	//Player* player = new Player(128, 128, 16, 16, true, "graywizard.png", 0, "Karl-Bertil");
	//currentWorld->AddPlayer(2,player); //MÅSTE FIXAS!!!!!!!!!!!!!
	//camera->setCameraAt(*player);

	//unsigned short i = 1;

	//currentWorld->RegisterBlock(i,(new BlockSolid(i))->RegisterBlock(i++));
	//+currentWorld->RegisterBlock(i,(new BlockBackground(i))->RegisterBlock(i++));
	//std::cin.get();

	//currentWorld->RegisterBlock(i,[&](unsigned short metadata) { return blockBackground; });
}

PlayState::~PlayState()
{
	//delete camera;
	//delete currentWorld;
	//delete blockMenu;
}

void PlayState::EventUpdate(App& app, sf::Event& event)
{
	currentWorld->EventUpdate(app, event);
	EventUpdate(app, event);
}

GameState *PlayState::Update(App& app)
{
	std::cout << 1/app.getFrameTime() << "\n";

	std::queue<sf::Packet>* packetDataList = currentWorld->Update(app, tC, camera);
	while (!packetDataList->empty())
	{
		connection->client->socket.send(packetDataList->front());
		packetDataList->pop();
	}
	//delete packetDataList;

	camera->Update(app);
	app.setView(*reinterpret_cast<sf::View*>(camera));
	
	blockMenu->Update(app, tC, *currentWorld);
	connection->Run();
	ProcessPackets();
	return this;
}

void PlayState::Draw(App& app)
{
	currentWorld->Draw(app, tC);
	//blockMenu->Draw(app, tC, *currentWorld);
}

void PlayState::ProcessPackets(void)
{
	connection->globalMutex.lock();
	std::queue<sf::Packet*> packets = connection->packets;
	connection->packets = std::queue<sf::Packet*>();
	connection->globalMutex.unlock();

	while(packets.size() > 0)
	{
		sf::Packet* packet = packets.front();
		//Now process packets
		sf::Uint16 packetType;
		sf::Uint16 wtf;
		if(!(*packet >> packetType))
			std::cout << "ERROR: Client could not extract data" << std::endl;
		//std::cout << "Client got packet " << packetType << " wtf?: " << wtf << std::endl;

		switch(packetType)
		{
		case InitMessage:
			{
				while(!packet->endOfPacket())
				{
					sf::Int16 ID;
					float xPos;
					float yPos;
					sf::Int16 sizeX;
					sf::Int16 sizeY;

					if(!(*packet >> ID  >> xPos >> yPos >> sizeX >> sizeY)) {}
						//std::cout << "ERROR: Client could not extract data" << std::endl;
					else
					{
						Player* player = new Player(xPos, yPos, 16, 16, false, "graywizard.png", 0, "temp", eventHandler);
						currentWorld->AddPlayer(ID, player);
					}
				}
			}
			break;
		case ClientID:
			{
				sf::Uint16 ID;
				sf::Uint16 blabla;
				if(!(*packet >> blabla >> ID))
					std::cout << "ERROR: Client could not extract data" << std::endl;
				connection->client->ID = ID;
				std::cout << "My ID is now " << ID << std::endl;
				sf::Packet send;
				sf::Uint16 packetType = PlayerJoinLeft;
				sf::Uint16 type = 0;
				float xPos = 0;
				float yPos = 0;
				send << packetType << type << xPos << yPos;
				connection->client->socket.send(send);
				std::cout << "Client sent PlayerJoinLeft " << packetType << " " << type << " " << xPos << " " << yPos << " " << connection->client->ID << std::endl;
			}
			break;
		case PingMessage: //measure ping between sent 1 and received 1 (type)
			{
				if (packetType == 1)
				{
					sf::Packet packet;
					sf::Uint16 type = 1;
					packet << type;
					connection->client->socket.send(packet);

				}
			}
			break;
		case KickMessage: //server kicks client (type, string message)

			break;
		case PlayerJoinLeft:
			{
				sf::Uint16 type;
				sf::Uint16 packetType;
				float xPos;
				float yPos;
				sf::Uint16 clientID;
				*packet >> type;

				//std::cout << "Client got PlayerJoinLeft " << packetType << " " << type << " " << xPos << " " << yPos << " " << clientID << std::endl;
				if(type == 0)
				{
					*packet >> xPos >> yPos >> clientID;
					Player* temp = new Player(xPos, yPos, 16, 16, false, "graywizard.png", 0, "temp", eventHandler);
					std::cout << "Added player -> clientid received " << clientID << " this clientid " << connection->client->ID << std::endl;

					if(clientID == connection->client->ID)
					{
						temp->isClientControlling = true;
						std::cout << "Camera set" << std::endl;
						camera->setCameraAt(temp);
					}

					currentWorld->AddPlayer(clientID, temp);
				}
				else if(type == 1)
				{
					*packet >> clientID;
					currentWorld->RemovePlayer(clientID);
					std::cout << "Client " << clientID << " has left" << std::endl;
				}
				break;
			}
		case PlayerMove:
			{
				//std::cout << "Client got playermove" << std::endl;
				sf::Int16 ID;
				float xPos;
				float yPos;
				float speedX;
				float speedY;
				float angle;
				float horizontal;
				float vertical;
				*packet >> ID  >> xPos >> yPos >> speedX >> speedY >> angle >> horizontal >> vertical;
				Player* p = currentWorld->GetPlayer(ID);
				if (p != nullptr && ID != connection->client->ID)
				{
					p->CreatureMove(xPos, yPos, speedX, speedY, angle, horizontal, vertical);
					//Player* temp = new Player(xPos, yPos, 16, 16, true, "graywizard.png", 0, "temp");
					//temp->setSpeedX(speedX);
					//temp->setSpeedY(speedY);
					//temp->setAngle(angle);
					//currentWorld->SetPlayer(client->ID, temp);
					//std::cout << "Moved player! :D" << std::endl;
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
				currentWorld->setBlockAndMetadataClientOnly(xPos, yPos, layer, id, metadata);
			}
			break;
		case BlockMetadataChange:
			{
				sf::Int32 xPos;
				sf::Int32 yPos;
				sf::Uint16 layer;
				sf::Uint16 metadata;
				*packet >> xPos >> yPos >> layer >> metadata;
				currentWorld->setBlockMetadataClientOnly(xPos, yPos, layer, metadata);
			}
			break;
			std::cout << packetType << std::endl;
		}
		delete packet;
		packets.pop();
	}
}
#endif