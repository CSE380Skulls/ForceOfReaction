/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	GameGraphics.cpp

	See GameGraphics.h for a class description.
*/

#include "stdafx.h"
#include "src\game\Game.h"
#include "src\game\IllegalArgumentException.h"
#include "src\graphics\GameGraphics.h"
#include "src\graphics\TextureManager.h"
#include "src\gsm\state\GameStateManager.h"
#include "src\gsm\world\World.h"
#include "src\gui\GameGUI.h"
#include "src\os\GameOS.h"
#include "src\text\GameText.h"
#include "src\text\TextFileWriter.h"

/*
	GameGraphics - Default constructor, nothing to initialize.
*/
GameGraphics::GameGraphics()
{
}

/*
	~GameGraphics - Destructor, it cleans up the render lists and texture
	managers. This should only be called when the application is closing.
*/
GameGraphics::~GameGraphics()
{
	delete guiRenderList;
	delete guiTextureManager;
	delete worldRenderList;
	delete worldTextureManager;
}

/*
	clearWorldTextures - When the game leaves a level we have to clear
	out these data structures. Calling clear on these will delete
	all the objects inside.
*/
void GameGraphics::clearWorldTextures()
{	
	// CLEAR LEVEL DATA STRUCURES
	worldTextureManager->clear();
	worldRenderList->clear();
}

/*
	fillRenderLists - This method causes the render lists to be 
	filled with the things that have to be drawn this frame.
*/
void GameGraphics::fillRenderLists(Game *game)
{
	// GENERATE RENDER LISTS FOR GAME WORLD AND GUI
	GameStateManager *gsm = game->getGSM();
	gsm->addGameRenderItemsToRenderList(game);
	GameGUI *gui = game->getGUI();
	gui->addRenderItemsToRenderList(game);
}

/*
	init - This method constructs the data structures for managing textures
	and render lists. It calls the createTextureManager, which is technology
	specific, and so is implemented only by child classes.
*/
void GameGraphics::init(int initScreenWidth, int initScreenHeight)
{
	// INIT SCREEN DIMENSIONS
	screenWidth = initScreenWidth;
	screenHeight = initScreenHeight;

	// GUI TEXTURES (like buttons, cursor, etc.)
	guiRenderList = new RenderList(DEFAULT_INIT_GUI_RENDER_LIST_SIZE);
	guiTextureManager = createTextureManager();

	// LEVEL TEXTURES (like sprites, tiles, particles, etc.)
	worldRenderList = new RenderList(DEFAULT_INIT_LEVEL_RENDER_LIST_SIZE);
	worldTextureManager = createTextureManager();
}

/*
	renderText - This method will go through the GameText argument,
	pull out each TextToDraw object, and use a technology-specific
	method in a child class, renderTextToDraw, to render each
	piece of text.
*/
void GameGraphics::renderText(GameText *text)
{
	int numTextObjects = text->getNumTextObjectsToDraw();
	for (int i = 0; i < numTextObjects; i++)
	{
		TextToDraw *textToDraw = text->getTextToDrawAtIndex(i);
		renderTextToDraw(textToDraw);
	}
}