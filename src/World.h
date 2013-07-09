#pragma once

#include <functional>
#include <vector>
#include <map>
#include <list>
#include <deque>
#include <queue>
#include "App.h"
#include <SFML\Network.hpp>

class Entity;
class Player;
class Block;
class Chunk;
class TextureContainer;

enum MessageType;

#define ChunkWidthHeight 256

/*struct Chunk
{
	Block *blocks[256][256];
};*/

typedef	std::pair<std::deque<Chunk*>, long> ChunkColumnType;
typedef std::pair<std::deque<ChunkColumnType>,long> ChunkMatrixType; // deque(y)<pair<deque<chunk>(x)>,long(chunkCenterY)>


class World
{
private:
	short int sizeX;
	short int sizeY;
	ChunkMatrixType chunkMatrix;
	//long chunkMatrixCenterRow;
	//std::pair<std::deque<std::pair<std::deque<Chunk*>,long>>,long> chunkMatrix;
	std::map<unsigned short, std::function<Block*(unsigned short)>> blockTypeMap;
	std::vector<Entity*> entityList;
	std::map<short, Player*> playerList;
	std::map<std::pair<short,short>,Block*> BlockMap;
	std::queue<sf::Packet>* packetDataList;
public:
	World();
#ifndef _SERVER
	void EventUpdate(App& app, sf::Event& event);
	void Draw(App& app, TextureContainer& tC);
#endif
	std::queue<sf::Packet>* Update(App& app, TextureContainer& tC);
	void RegisterBlock(unsigned short key, std::function<Block*(unsigned short)> value);
	void setBlock(long x, long y, long layer, unsigned short id);
	void setBlockAndMetadata(long x, long y, long layer, unsigned short id, unsigned short metadata);
	void setBlockMetadata(long x, long y, long layer, unsigned short metadata);
	MessageType setBlockAndMetadataClientOnly(long x, long y, long layer, unsigned short id, unsigned short metadata);
	MessageType setBlockMetadataClientOnly(long x, long y, long layer, unsigned short metadata);
	Block* getBlock(long x, long y, long layer);
	void Expand(long x, long y, Chunk* chunk);
	void AddBlockType(unsigned short, std::function<Block*(unsigned short)>);
	bool isBlockSolid(long x, long y);
	Block* getBlockType(unsigned short id, unsigned short metadata);
	std::map<unsigned short, std::function<Block*(unsigned short)>>& getBlockTypeMap();
	int AddEntity(Entity*);
	void RemoveEntity(int id);
	int AddPlayer(int id, Player*);
	void RemovePlayer(int id);
	Player* GetPlayer(int id);
	void SetPlayer(int id, Player* player);
};

/*#include <functional>
#include <vector>
#include <map>
#include <list>
#include <deque>
#include <SFML\System.hpp>
 
#ifndef _SERVER
#include <SFML\Graphics.hpp>
#include "App.h"
#else
class App;
#endif

class Chunk;
class Block;
class Entity;
class Player;
class TextureContainer;
class Camera;

#define SIZEXMAX 1024
#define SIZEYMAX 1024

#define SCREENSIZEX 800
#define SCREENSIZEY 600

class World
{
private:
	short int sizeX;
	short int sizeY;
	Chunk *chunkList[256][256];
	std::map<unsigned short, std::function<Block*(unsigned short)>> blockTypeMap;
	std::vector<Entity*> entityList;
	std::vector<Player*> playerList;

public:
	World(unsigned short, unsigned short);
	~World(void);
#ifndef _SERVER
	void Draw(App& app, TextureContainer& tC);
#endif
	void Update(App& app);
	void setBlock(unsigned char layer, short x, short y, unsigned short id);
	void setBlockAndMetadata(unsigned char layer, short x, short y, unsigned short id, unsigned short metadata);
	void setBlockMetadata(unsigned char layer, short x, short y, unsigned short metadata);
	void RegisterBlock(unsigned short, std::function<Block*(unsigned short)>);
	void DrawBorder(int blockId);
	bool isVisible(App& app, Entity& entity, short position);
	sf::Vector2i getSize();
	Block* getBlock(unsigned char layer, short x, short y);
	bool isBlockSolid(short x,short y);
	Block* getBlockType(unsigned short id, unsigned short metadata);
	std::map<unsigned short, std::function<Block*(unsigned short)>>* getBlockTypeMap();
	void AddEntity(Entity*);
	void AddPlayer(Player*, short Id);
	void RemovePlayer(short Id);
};*/