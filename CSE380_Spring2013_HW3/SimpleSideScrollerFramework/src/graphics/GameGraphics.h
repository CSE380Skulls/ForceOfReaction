/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	This class provides a framework for technology-specific
	classes that will manage the rendering of all game textures and text.
	It provides methods for the initialization of objects necessary
	for rendering images and text.

	Rendering should be done by generating a list of RenderItem
	objects each frame and putting them into a list. These
	objects store information about the textures that have
	to be drawn to the screen.
*/

#pragma once

#include "stdafx.h"
#include "src\game\Game.h"
#include "src\game\WStringTable.h"
#include "src\graphics\RenderList.h"
#include "src\graphics\TextureManager.h"
#include "src\gsm\state\GameStateManager.h"
#include "src\gui\GameGUI.h"
#include "src\input\GameInput.h"
#include "src\text\GameText.h"
#include "src\text\TextFileWriter.h"

const static unsigned int DEFAULT_INIT_GUI_RENDER_LIST_SIZE = 100;
const static unsigned int DEFAULT_INIT_LEVEL_RENDER_LIST_SIZE = 3000;

class TextureManager;

class GameGraphics
{
protected:
	// WE PERIODICALLY NEED ACCESS TO GAME STUFF
	Game *game;

	// WE'LL SORE THE SCREEN DIMENSIONS HERE
	int screenWidth;
	int screenHeight;

	// EACH FRAME, RENDER LISTS ARE CREATED, FILLED WITH
	// DATA, THEN USED FOR RENDERING THE FRAME, AND DESTROYED
	RenderList				*guiRenderList;
	RenderList				*worldRenderList;

	// NOTE THAT IF WE REALLY WANTED TO BE SLICK, WE COULD PROVIDE
	// MULTIPLE WORLD RENDER LISTS, ONE FOR EACH Z-LEVEL

	// THE TextureManagers STORE ALL THE IMAGES FOR OUR GAME

	TextureManager			*guiTextureManager;
	TextureManager			*worldTextureManager;

public:	
	// INLINED ACCESSOR METHODS
	RenderList*				getGUIRenderList()			{ return guiRenderList;			}
	TextureManager*			getGUITextureManager()		{ return guiTextureManager;		}
	RenderList*				getWorldRenderList()		{ return worldRenderList;		}
	TextureManager*			getWorldTextureManager()	{ return worldTextureManager;	}

	// VIRTUAL METHODS TO BE OVERRIDDEN BY TECHNOLOGY
	// SPECIFIC CLASSES
	virtual	TextureManager* createTextureManager()=0;
	virtual int				getScreenHeight()=0;
	virtual int				getScreenWidth()=0;
	virtual	void			initGraphics(GameOS *os, bool isFullscreen)=0;
	virtual void			initTextFont(int fontSize)=0;
	virtual void			reloadGraphics()=0;
	virtual void			renderGame(Game *game)=0;
	virtual void			renderTextToDraw(TextToDraw *textToDraw)=0;
	virtual void			setColorKey(int r, int g, int b)=0;
	virtual void			setFontColor(int r, int g, int b)=0;
	virtual void			shutdown()=0;

	// CONCRETE METHODS DEFINED IN GameGraphics.cpp
	GameGraphics();
	~GameGraphics();
	void clearWorldTextures();
	void fillRenderLists(Game *game);
	void init(int screenWidth, int screenHeight);
	void renderText(GameText *text);
};