#include "World.h"
#include "App.h"
#include "TextureContainer.h"
#include "Camera.h"
#include "Chunk.h"
#include "BlockSolid.h"
#include "BlockBackground.h"
#include "Player.h"
#include "MessageType.h"
#include "EventHandler.h"
#include <deque>


#define getChunkMatrixIndexX(x) (int)abs(x>>4 + chunkMatrix.second)
#define getChunkColumnIndexY(y,x_it) (int)abs(y>>4 + x_it.second)
#define isColumnInsideChunkMatrix(x) x >= 0 && x < chunkMatrix.first.size()
#define isChunkInsideChunkColumn(y,x_it) y >= 0 && y < x_it.size()
//(int)floor(camera.GetCenter().x+camera.GetHalfSize().x)
//floor(camera.GetCenter().y-camera.GetHalfSize().y)
World::World()
{
	unsigned short i = 1;

	RegisterBlock(i,(new BlockSolid(i))->RegisterBlock(i));i++;
	RegisterBlock(i,(new BlockBackground(i))->RegisterBlock(i));i++;

	for (int x = 0; x < 3; x++)
	{ 
	chunkMatrix.first.push_back(ChunkColumnType(std::deque<Chunk*>(),0));
	for (int y = 0; y < 3; y++)
	{
	Chunk* chunk = new Chunk();
	chunk->setBlock(2, 1, 1, new  BlockSolid(1));//(*getBlockType(1))(0));
	chunkMatrix.first[x].first.push_back(chunk);
	}
	}

	packetDataList = new std::queue<sf::Packet>();
}

#ifndef _SERVER

void World::EventUpdate(App& app, sf::Event& event)
{
	for (Entity* entity : entityList)
	{
		entity->EventUpdate(app, event, this, packetDataList);
	}

	for(std::pair<short, Player*> pair : playerList)
	{
		pair.second->EventUpdate(app, event, this, packetDataList);
	}
}

void World::Draw(App& app, TextureContainer& tC)
{
	for (int x = 0; x < chunkMatrix.first.size(); x++)
	{
		for (int y = 0; y < chunkMatrix.first[x].first.size(); y++)
		{
			if (chunkMatrix.first[x].first[y] != nullptr)
			{
				chunkMatrix.first[x].first[y]->Draw(x-chunkMatrix.second, y-chunkMatrix.first[x].second, app, tC);
			}
		}
	}

	for (Entity* entity : entityList)
	{
		entity->Draw(app, tC);
	}

	for(std::pair<short, Player*> pair : playerList)
	{
		pair.second->Draw(app, tC);
	}
}
#endif

std::queue<sf::Packet>* World::Update(App& app, TextureContainer& tC, Camera* camera)
{
	for (Entity* entity : entityList)
	{
#ifdef _SERVER
		entity->Update(app, this, packetDataList, camera);
#else
		entity->Update(app, this, packetDataList, camera, eventHandler);
#endif
	}

	for(std::pair<short, Player*> pair : playerList)
	{
#ifdef _SERVER
		pair.second->Update(app, this, packetDataList, camera);
#else
		pair.second->Update(app, this, packetDataList, camera, eventHandler);
#endif
	}	

	return packetDataList;
	packetDataList = new std::queue<sf::Packet>();
}

void World::RegisterBlock(unsigned short key, std::function<Block*(unsigned short)> value)
{
	blockTypeMap.emplace(key, value);
}

/*#ifndef _SERVER
void World::Draw(App& app, TextureContainer& tC)
{
for (auto chunkColumn : chunkMatrix.first)
{
for (auto chunk : chunkColumn.first)
{
chunk->Draw(0, 0, app, tC);
}
}
/*ChunkColumnType::size_type x,y;
for (x = (int)abs((int)floor(GetCamera(app).GetCenter().x-GetCamera(app).GetHalfSize().x)>>4 + chunkMatrixCenterRow);
x != chunkMatrix.size && x < (int)abs((int)floor(GetCamera(app).GetCenter().x+GetCamera(app).GetHalfSize().x)>>4 + chunkMatrixCenterRow);
x++)
{
for (y = (int)abs((int)floor(GetCamera(app).GetCenter().y-GetCamera(app).GetHalfSize().y)<<4 + chunkMatrix.at(x).second);
y != chunkMatrix.size && y < (int)abs((int)floor(GetCamera(app).GetCenter().y+GetCamera(app).GetHalfSize().y)>>4 + chunkMatrix.at(x).second);
y++)
{
chunkMatrix.at(x).first.at(y)->Draw(x-(int)(GetCamera(app).GetCenter().x-GetCamera(app).GetHalfSize().x), y-(int)(GetCamera(app).GetCenter().y-GetCamera(app).GetHalfSize().y), app, tC);
}
}*/
//}
//#endif

void World::setBlock(long x, long y, long layer, unsigned short id)
{
	setBlockAndMetadata(x, y, layer, id, 0);
}

void World::setBlockAndMetadata(long x, long y, long layer,  unsigned short id, unsigned short metadata)
{
	/*if (setBlockAndMetadataClientOnly(x, y, layer, id, metadata))
	{
		sf::Packet packet;
		packet << (sf::Int16)BlockPlace << (sf::Int32)x << (sf::Int32)y << (sf::Uint16)layer << (sf::Uint16)id << (sf::Uint16)metadata;

		packetDataList->push(packet);
	}
	else
	{
		setBlockMetadata(x, y, layer, metadata);
	}*/

	MessageType messageType = setBlockAndMetadataClientOnly(x, y, layer, id, metadata);

	switch (messageType)
	{
	case NullMessage:
		break;

	case BlockPlace:
		{
			sf::Packet packet;
			packet << (sf::Int16)BlockPlace << (sf::Int32)x << (sf::Int32)y << (sf::Uint16)layer << (sf::Uint16)id << (sf::Uint16)metadata;
			packetDataList->push(packet);
		}
		break;

	case BlockMetadataChange:
		{
			sf::Packet packet;
			packet << (sf::Int16)BlockMetadataChange << (sf::Int32)x << (sf::Int32)y << (sf::Uint16)layer << (sf::Uint16)metadata;
			packetDataList->push(packet);
		}
		break;

	default:
		{
			std::cout << "Unexpected Messagetype: " << messageType << "\n";
		}
		break;
	}
}

void World::setBlockMetadata(long x, long y, long layer, unsigned short metadata)
{
	if (setBlockMetadataClientOnly(x, y, layer, metadata))
	{
		sf::Packet packet;
		packet << (sf::Int16)BlockMetadataChange << (sf::Int32)x << (sf::Int32)y << (sf::Uint16)layer << (sf::Uint16)metadata;

		packetDataList->push(packet);
	}
}

MessageType World::setBlockAndMetadataClientOnly(long x, long y, long layer, unsigned short id, unsigned short metadata)
{
	long xx = floor(x * 0.0625);

	unsigned short xxx = x&0xF;//(x < 0)? (abs(x+1)&0xF)^0xF : x&0XF;
	unsigned short yyy = y&0xF;//(y < 0)? y&0XF : y&0XF;

	//std::cout << xx <<"?\n";
	//if (!isColumnInsideChunkMatrix(xx+chunkMatrix.second))
	{
		Expand(xx, 0, nullptr);
	}
	{
		//std::cout << "NU?\n";
		long yy = floor(y * 0.0625);

		//if (!isChunkInsideChunkColumn(yy + it.second,it.first))
		{
			Expand(xx, yy, nullptr);
		}

		auto &it = chunkMatrix.first.at(xx + chunkMatrix.second);

		{
			//std::cout << xx << " " << yy << " " << xx+chunkMatrix.second << " " << yy+it.second << " s " << chunkMatrix.first.size() << " " << it.first.size() <<" JAHA?\n";
			Chunk* c = it.first.at(yy + it.second);
			if (c == nullptr)
			{
				if (id == 0)
					return NullMessage;

				c = it.first.at(yy + it.second) = new Chunk();
			}
			else
			{
				Block* block = c->getBlock(layer, xxx, yyy);
				//printf(" %X %d %d\n", block, xxx, yyy);
				if (block != nullptr)
				{
					if (block->getId() == id)
					{
						if (c->getMetadata(layer, xxx, yyy) != metadata)
						{
							c->setMetadata(layer, xxx, yyy, metadata);
							return BlockMetadataChange;
						}

						return NullMessage;
					}
				}
				else if (id == 0)
				{
					return NullMessage;
				}
			}

			c->setBlock(layer, xxx, yyy, getBlockType(id,metadata));
			c->setMetadata(layer, xxx, yyy, metadata);
			return BlockPlace;
		}
	}

	return NullMessage;
}

MessageType World::setBlockMetadataClientOnly(long x, long y, long layer, unsigned short metadata)
{
	unsigned short xxx = x&0xF;//(x < 0)? (abs(x+1)&0xF)^0xF : x&0XF;
	unsigned short yyy = y&0xF;//(y < 0)? y&0XF : y&0XF;

	long xx = floor(x * 0.0625);
	if (isColumnInsideChunkMatrix(xx))
	{
		auto &it = chunkMatrix.first.at(xx);
		long yy = floor(y * 0.0625);
		if (isChunkInsideChunkColumn(yy,it.first))
		{
			Chunk *c = it.first.at(yy);

			if (c != nullptr)
			{
				if (c->getMetadata(layer, xxx, yyy) != metadata)
				{
					c->setMetadata(layer, xxx, yyy, metadata);
					return BlockMetadataChange;
				}
			}
		}
	}
	return NullMessage;
}

Block* World::getBlock(long x, long y, long layer)
{
	long xx = floor(x * 0.0625) + chunkMatrix.second;

	unsigned short xxx = x&0xF;
	unsigned short yyy = y&0xF;

	if (isColumnInsideChunkMatrix(xx))
	{
		auto it = chunkMatrix.first.at(xx);
		long yy = floor(y * 0.0625) + it.second;

		if (isChunkInsideChunkColumn(yy,it.first))
		{
			if (it.first.at(yy) != nullptr)
			{
				return it.first.at(yy)->getBlock(layer, xxx, yyy);
			}
		}
	}
	return nullptr;
}

void World::Expand(long x, long y, Chunk* chunk)
{
	//std::cout << sizeof(int);
	long xx = x + chunkMatrix.second;
	if (xx < 0)
	{
		for (long i = xx; i < 0; i++)
		{
			//std::cout << "XD\n";
			chunkMatrix.first.push_front(ChunkColumnType(std::deque<Chunk*>(),0));
			chunkMatrix.second++;
			//std::cout << "XD XD XD\n";
		}
		xx = x + chunkMatrix.second;
	}
	else if (xx >= chunkMatrix.first.size())
	{
		for (long i = 0; i <= xx; i++)
		{
			chunkMatrix.first.push_back(ChunkColumnType(std::deque<Chunk*>(),0));
		}
	}

	ChunkColumnType& it = chunkMatrix.first.at(xx);
	long yy = y + it.second;

	if (yy < 0)
	{
		for (long i = yy; i < 0; i++)
		{
			it.first.push_front(nullptr);
			it.second++;
		}
		yy = y + it.second;
	}
	else if (yy >= it.first.size())
	{
		for (long i = 0; i <= yy; i++)
		{
			it.first.push_back(nullptr);
		}
	}
}

void World::AddBlockType(unsigned short key, std::function<Block*(unsigned short)> value)
{
	blockTypeMap.emplace(key, value);
}

Block* World::getBlockType(unsigned short id, unsigned short metadata)
{
	auto it = blockTypeMap.find(id);
	return (it == blockTypeMap.end())? nullptr : it->second(metadata);
}

std::map<unsigned short, std::function<Block*(unsigned short)>>& World::getBlockTypeMap()
{
	return blockTypeMap;
}

int World::AddEntity(Entity* entity)
{
	entityList.push_back(entity);
	return 0;
}

void World::RemoveEntity(int id)
{

}

int World::AddPlayer(int id, Player* player)
{
	auto it = playerList.find(id);
	if(it == playerList.end())
	{
		playerList.insert(std::pair<short, Player*>(id, player));
	}
	else
		std::cout << "Attempt to add player that already exists! " << id << std::endl;

	return 0;
}

void World::RemovePlayer(int id)
{
	auto it = playerList.find(id);
	if(it != playerList.end())
	{
		delete(it->second);
		playerList.erase(id);
	}
	else
		std::cout << "Attempt to remove player that doesn't exist! " << id << std::endl;
}

Player* World::GetPlayer(int id)
{
	auto it = playerList.find(id);
	return (it == playerList.end()) ? nullptr : it->second;
}

void World::SetPlayer(int id, Player* player)
{
	delete(playerList.find(id)->second);
	playerList.erase(id);
	playerList.insert(std::pair<short, Player*>(id, player));
}

bool World::isBlockSolid(long x,long y)
{
	Block* block = getBlock(x+16, y+16, 2);
	if (block != nullptr)
	{
		return block->isSolid();
	}
	return false;
}

/*#include "World.h"
#include "Entity.h"
#include "Player.h"
#include "projectile.h"
#include "camera.h"
#include "Chunk.h"
#include "Block.h"
#include "BlockSolid.h"
#include "BlockBackground.h"
#include "App.h"

#ifndef _SERVER
#include <SFML\Graphics.hpp>
#endif

World::World(short unsigned sizeX, short unsigned sizeY)
{
unsigned short i = 1;

RegisterBlock(i,(new BlockSolid(i))->RegisterBlock(i));i++;
RegisterBlock(i,(new BlockBackground(i))->RegisterBlock(i));i++;

this->sizeX = sizeX+1;
this->sizeY = sizeY+1;

if(this->sizeX > SIZEXMAX)
this->sizeX = SIZEXMAX;
if(this->sizeY > SIZEYMAX)
this->sizeY = SIZEYMAX;

for(int x = 0; x < this->sizeX; x++)
{
for(int y = 0; y < this->sizeY; y++)
{
chunkList[x][y] = new Chunk();
}
}

DrawBorder(2);
}

World::~World(void)
{

}

void World::DrawBorder(int blockId)
{
for(int xx = 0; xx < this->sizeX * 16; xx++)
{
setBlock(2, 256 + xx * 16, 256, 1);
setBlock(2, 256 + xx * 16, sizeY * 16 * 16 - 16, 1);
}
for(int yy = 0; yy < this->sizeY * 16; yy++)
{
setBlock(2, 256, 256 + yy * 16, 1);
setBlock(2, sizeX * 16 * 16 - 16, 256 + yy * 16, 1);
}
}

void World::Update(App& app)
{
for (std::vector<Entity*>::size_type i = 0; i < entityList.size(); i++)
{
entityList[i]->Update(app, *this);
}
for (std::vector<Player*>::size_type i = 0; i < playerList.size(); i++)
{
playerList[i]->Update(app, *this);
}
}

#ifndef _SERVER
void World::Draw(App& app, TextureContainer& tc)
{
float cameraX = (*reinterpret_cast<const Camera*>(&app.GetView())).GetCenter().x;
float cameraY = GetCamera(app).GetCenter().y;

int chunkX = (int)((cameraX/16)/16);
int chunkY = (int)((cameraY/16)/16);

for(short x = -2; x < 4; x++)
{
for(short y = -2; y < 4; y++)
{
if(chunkX+x > 0 && chunkY+y > 0 && chunkX+x < sizeX && chunkY+y < sizeY)
{
chunkList[chunkX + x][chunkY + y]->Draw(chunkX + x, chunkY + y, app, tc);
}
}
}

//Projectile start


//Entity start
for (std::vector<Entity>::size_type i = 0; i < entityList.size(); i++)
{
if(isVisible(app, *entityList[i], i))
{
entityList[i]->Draw(app, tc);
}
}

//Player start
for (std::vector<Player*>::size_type i = 0; i < playerList.size(); i++)
{
if(isVisible(app, *playerList[i], i))
{
playerList[i]->Draw(app, tc);
}
}
}
#endif

void World::setBlock(unsigned char layer, short x, short y, unsigned short id)
{
setBlockAndMetadata(layer, x, y, id, 0);
}

void World::setBlockAndMetadata(unsigned char layer, short x, short y, unsigned short id, unsigned short metadata)
§{
int chunkX = (x/16)/16;
int chunkY = (y/16)/16;
int blockX = (x/16) % 16;
int blockY = (y/16) % 16;
if(chunkX*16 + blockX >= 0 && chunkX*16 + blockX < sizeX*16 && chunkY*16 + blockY>= 0 && chunkY*16 + blockY < sizeY*16)
{
chunkList[chunkX][chunkY]->setBlock(layer, blockX, blockY, getBlockType(id, metadata));
chunkList[chunkX][chunkY]->setMetadata(layer, blockX, blockY, metadata);
}
}

void World::setBlockMetadata(unsigned char layer, short x, short y, unsigned short metadata)
{
int chunkX = (x/16)/16;
int chunkY = (y/16)/16;
int blockX = (x/16) % 16;
int blockY = (y/16) % 16;
if(chunkX*16 + blockX >= 0 && chunkX*16 + blockX < sizeX*16 && chunkY*16 + blockY>= 0 && chunkY*16 + blockY < sizeY*16)
{
chunkList[chunkX][chunkY]->setMetadata(layer, blockX, blockY, metadata);
}
}


Block* World::getBlock(unsigned char layer, short x, short y)
{
/*x+=16;
y+=16;
int chunkX = x>>4;
int chunkY = y>>4;
int blockX = x&0xF;
int blockY = y&0xF;
if(x >= 0 && x < sizeX<<4 && y >= 0 && y < sizeY<<4)
{
return(chunkList[chunkX][chunkY]->getBlock(layer, blockX, blockY));
}
else
{
return(nullptr);
}* /
return nullptr;
}

bool World::isBlockSolid(short x,short y)
{
Block* block = getBlock(2,x,y);

if (block == nullptr)
return false;

return block->isSolid();


//return block->isSolid();
//return block->isSolid();
/*Block* block = getBlock(2,x,y);

if (block == nullptr)
return false;

return block->isSolid();* /
}

void World::RegisterBlock(unsigned short key, std::function<Block*(unsigned short)> value)
{
blockTypeMap.emplace(key, value);
}

Block* World::getBlockType(unsigned short id, unsigned short metadata)
{
auto it = blockTypeMap.find(id);
return ((it == blockTypeMap.end()) ? nullptr : it->second(metadata));
}

std::map<unsigned short, std::function<Block*(unsigned short)>>* World::getBlockTypeMap()
{
return &blockTypeMap;
}

sf::Vector2i World::getSize(){return(sf::Vector2i(sizeX, sizeY));};

#ifndef _SERVER
bool World::isVisible(App& app, Entity& entity, short position)
{
float posX = entity.getPosition().x;
float posY = entity.getPosition().y;
float sizeX = entity.getSize().x;
float sizeY = entity.getSize().y;

if((posX + (2*sizeX)) >= -16 && (posY + (2*sizeY)) >= -16)
{
if((posX - sizeX) <= this->sizeX*256 && (posY - sizeY) <= this->sizeY*256)
{
if((posX + 2*sizeX) >= (GetCamera(app).GetCenter().x - GetCamera(app).GetHalfSize().x))
{
if((posY + 2*sizeY) >= (GetCamera(app).GetCenter().y - GetCamera(app).GetHalfSize().y))
{
if(posX - sizeX <= (GetCamera(app).GetCenter().x + GetCamera(app).GetHalfSize().x))
{
if(posY - sizeY <= (GetCamera(app).GetCenter().y + GetCamera(app).GetHalfSize().y))
{
return true;
}
}
}
}
return false;
}
}
return false;
//delete entityList[position];
}
#endif

void World::AddEntity(Entity* creature)
{
entityList.push_back(creature);
}

void World::AddPlayer(Player* player, short Id)
{
playerList.insert((playerList.begin() + Id), player);
}

void World::RemovePlayer(short Id)
{
playerList.erase(playerList.begin() + Id);
}*/