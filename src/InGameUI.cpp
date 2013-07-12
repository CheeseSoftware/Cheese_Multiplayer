#ifndef _SERVER
#include "InGameUI.h"
#include "TextureContainer.h"
#include "World.h"
#include "camera.h"
#include "Block.h"
#include "BlockSolid.h"
#include "BlockBackground.h"
#include <iostream>

InGameUI::InGameUI(App &app, TextureContainer &tc, World &world)
{
	tc.AddSpriteSheet("UIMainStrip.png", 768, 24);
	tc.AddSpriteSheet("UIBlockBackground.png", 768, 128);
	tc.AddSpriteSheet("UIMainButtonBlocks.png", 64, 18);
	selectedBlockSolid = 0;
	selectedBackground = 0;

	sf::Vector2i drawPos(1152-420, 720-170);
	auto chatBox = app.add<tgui::TextBox>("chatBox");
	auto editBox = app.add<tgui::EditBox>("chatBoxInput");

	if(!chatBox->load(340, 172, 24, "TGUI/objects/Scrollbar/Black"))
		std::cout << "error";
	if(!editBox->load("TGUI/objects/EditBox/Black"))
		std::cout << "error";

	chatBox->setBorders(2, 2, 2, 2);
	editBox->setBorders(2, 2, 2, 2);
	
	chatBox->selectionPointWidth = 2;
	chatBox->selectionPointColor = sf::Color::Black;

	editBox->setSize(340, 40);
	editBox->setMaximumCharacters(100);
	editBox->enable();

	chatBox->setPosition(drawPos.x + 0, drawPos.y + 0 );
	editBox->setPosition(drawPos.x + 0, drawPos.y + 170);
}


InGameUI::~InGameUI(void)
{
}

void InGameUI::Update(App& app, TextureContainer &tc, World &world)
{
	sf::Vector2f bla = app.getView().getCenter() - sf::Vector2f(app.getView().getSize().x/2, app.getView().getSize().y/2);
	sf::Vector2f mousePos = sf::Vector2f(
		app.getView().getCenter().x 
		- (app.getView().getSize().x/2)
		+ sf::Mouse::getPosition().x
		- (8*16),
		+ app.getView().getCenter().y
		- (app.getView().getSize().y/2)
		+ sf::Mouse::getPosition().y);

	sf::Event event;
	while(false)//while(app.pollEvent(event)) <- läs sfml-tutorials... så kan man absolut inte göra >.< använd eventupdate!
	{
		if(event.type == sf::Event::MouseButtonPressed)
		{
			if(event.key.code == sf::Mouse::Left)
			{
				for(int i = 0; i < 16; i++)
				{
					if(mousePos.x > 96 + bla.x + 16 * i - 128 && mousePos.x < 96 + bla.x + 16 * i + 16 -128 && mousePos.y > bla.y + 492 && mousePos.y < bla.y + 492 + 16)
					{
						selectedBackground = -1;
						selectedBlockSolid = i;
						std::cout << "Clicked block solid " << i << std::endl;
					}
				}
				for(int i = 0; i < 16; i++)
				{
					if(mousePos.x > 96 + bla.x + 16 * i + 144 && mousePos.x < 96 + bla.x + 16 * i + 16 + 128 + 16 && mousePos.y > bla.y + 492 && mousePos.y < bla.y + 492 + 16)
					{
						selectedBlockSolid = -1;
						selectedBackground = i;
						std::cout << "Clicked block background " << i << std::endl;
					}
				}
			}
			if(event.key.code == sf::Mouse::Left)
			{
				if(sf::Mouse::getPosition().y < 512 - 24)
				{
					//ORKAif(selectedBlockSolid != -1)
					//ORKAworld.setBlockAndMetadata((long)((app.getView().GetCenter().x + app.GetInput().GetMouseX()-(8*16))*0.0625), (long)(app.getView().GetCenter().y + app.GetInput().GetMouseY())>>4, 2, 1, selectedBlockSolid);//world.setBlockAndMetadata(2,(short)( app.getView().GetCenter().x + app.GetInput().GetMouseX()-(8*16)), (short)(app.getView().GetCenter().y + app.GetInput().GetMouseY()), 1, selectedBlockSolid);
					//ORKAelse if(selectedBackground != -1)
					//ORKAworld.setBlockAndMetadata((long)((app.getView().GetCenter().x + app.GetInput().GetMouseX()-(8*16))*0.0625), (long)(app.getView().GetCenter().y + app.GetInput().GetMouseY())>>4, 0, 2, selectedBackground);

					//world.setBlockAndMetadata(0, (short)(app.getView().GetCenter().x + app.GetInput().GetMouseX()-(8*16)), (short)(app.getView().GetCenter().y + app.GetInput().GetMouseY()), 2, selectedBackground);
				}
			}
			else if(event.key.code == sf::Mouse::Right)
			{
				int layer = 2;
				//if (world.getBlock(layer, (short)(app.getView().GetCenter().x + app.GetInput().GetMouseX()-(8*16))>>4, (short)(app.getView().GetCenter().y + app.GetInput().GetMouseY())>>4) == 0)
				//	layer = 0;
				//ORKAworld.setBlock((short)(app.getView().GetCenter().x + app.GetInput().GetMouseX()-(8*16))>>4, (short)(app.getView().GetCenter().y + app.GetInput().GetMouseY())>>4, 2, 0);	
			}
		}
	}
}

void InGameUI::Draw(App& app, TextureContainer &tc, World &world)
{
	sf::Vector2f bla = app.getView().getCenter() - sf::Vector2f(app.getView().getSize().x/2, app.getView().getSize().y/2);

	sf::Sprite *mainStripSprite = &(tc.getTextures("UIMainStrip.png")[0]);
	sf::Sprite *buttonBlocksSprite = &(tc.getTextures("UIMainButtonBlocks.png")[0]);

	if (mainStripSprite != 0 && buttonBlocksSprite != 0)
	{
		mainStripSprite->setPosition(bla.x + 0, bla.y + 512 - 24);
		buttonBlocksSprite->setPosition(bla.x + 1, bla.y + 512 - 21);

		app.draw(*mainStripSprite);
		app.draw(*buttonBlocksSprite);
		//app.draw(*chatBox);
		app.drawGUI();
	}

	for (auto b : world.getBlockTypeMap())//for (std::map<unsigned short, std::function<Block*(unsigned short)>>::iterator b = world.getBlockTypeMap()->begin(); b != world.getBlockTypeMap()->end(); b++)
	{
		//return;
		for(int i = 0; i < 32; i++)
		{
			Block* block = b.second(i);
			if(block->getLayer() == 2 && i < 18)
			{
				block->Draw((i<<4)+bla.x + 96, 1+bla.y+492, app, tc, i);
				//sf::Sprite *tempSprite = &(tc.getTextures(block->getTextureName())[block->getTextureId(app, 0)]);
				//tempSprite->SetPosition(96 + bla.x + 16 * i, bla.y + 492);
				//app.Draw(*tempSprite);
			}
			else if(block->getLayer() == 0 && i >= 18)
			{
				block->Draw((i<<4)+bla.x + 96, 1+bla.y+492, app, tc, i-18);
				//sf::Sprite *tempSprite = &(tc.getTextures(blockk->getTextureName())[blockk->getTextureId(app, 0)]);
				//tempSprite->SetPosition(96 + bla.x + 16 * i + 16, bla.y + 492);
				//app.Draw(*tempSprite);
			}
		}
	}
}
#endif