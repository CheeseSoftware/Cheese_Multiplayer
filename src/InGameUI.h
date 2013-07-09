#ifndef _SERVER
class InGameUI;
#pragma once
#include <SFML\Graphics.hpp>
#include <TGUI\TGUI.hpp>
#include "App.h"

class TextureContainer;
class World;
class Camera;
class Block;

class InGameUI
{
public:
	InGameUI(App &app, TextureContainer &tc, World &world);
	~InGameUI(void);
	void Update(App& app, TextureContainer &tc, World &world);
	void Draw(App& app, TextureContainer &tc, World &world);
	std::map<Block*, sf::Vector2f> InGameUIItemList;
	short int selectedBlockSolid;
	short int selectedBackground;

};
#endif