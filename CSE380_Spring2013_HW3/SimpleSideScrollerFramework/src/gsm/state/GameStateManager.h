/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	GameStateManager.h

	This class manages the static data for the game level being
	used. This means all game backgrounds, which are stored and
	manipulated in WorldLayer objects. A given level can have
	a game background rendered using multiple layers, some which
	can be tiled, others that can be sparse, and others that
	can be isometric.

	This class stores these layers and ensures they are rendered
	in the proper order, low index to high. Layers that need to
	be drawn first (the back-most layer), should be added first.
*/
#pragma once
#include "stdafx.h"
#include "src\dataLoader\GameDataLoader.h"
#include "src\graphics\RenderList.h"
#include "src\gsm\state\GameState.h"
#include "src\gsm\physics\Physics.h"
#include "src\gsm\physics\BoxPhysics.h"
#include "src\gsm\sprite\SpriteManager.h"
#include "src\gsm\world\World.h"
#include "src\gsm\world\WorldLayer.h"

class Game;

static const int NO_LEVEL_LOADED = 0;

class GameStateManager
{
private:
	// THE CURRENT GAME STATE
	GameState currentGameState;

	// THE CURRENT LEVEL IS THE ONE CURRENTLY BEING PLAYED. NOTE THAT IF
	// THE CURRENT LEVEL IS 0, THEN IT MEANS THERE IS NO ACTIVE LEVEL LOADED,
	// LIKE WHEN WE'RE AT THE MAIN MENU
	unsigned int currentLevel;

	// THESE VECTORS STORE THE NAMES OF EACH LEVEL AND THE RELATIVE
	// PATH AND FILE NAME OF EACH DATA INPUT FILE FOR ALL GAME LEVELS. 
	// NOTE THAT WE'LL LOAD THESE VECTORS WITH THIS DATA WHEN THE GAME STARTS UP, 
	// BUT WE'LL ONLY LOAD THE LEVELS INTO THE GAME WORLD AS EACH LEVEL IS PLAYED
	// NOTE THAT THE LEVEL NAME AT INDEX i IN THE FIRST VECTOR CORRESPONDS
	// TO THE PATH AND FILE NAME AT INDEX i IN THE SECOND ONE
	vector<wstring> levelNames;
	vector<wstring> levelFileNamesWithRelativePath;

	// FOR MANAGING STATIC GAME WORLD DATA, i.e. BACKGROUND IMAGES,
	// TILES, PLATFORMS, etc. BASICALLY THINGS THAT ARE NOT ANIMATED
	// AND DO NOT MOVE
	World world;

	// FOR MANAGING DYNAMIC GAME OBJECTS FOR CURRENT LEVEL
	// NOTE THAT WE CALL THE DYNAMIC OBJECTS "SPRITES"
	SpriteManager *spriteManager;

	// FOR DOING ALL COLLISION DETECTION AND RESOLUTION
	Physics			physics;
	
	//This is the Box 2d Physics engine
	BoxPhysics		boxPhysics;

public:
	// INLINED ACCESSOR METHODS
	GameState		getCurrentGameState()	{ return currentGameState;			}
	unsigned int	getCurrentLevel()		{ return currentLevel;				}
	unsigned int	getNumLevels()			{ return levelNames.size();			}
	Physics*		getPhysics()			{ return &physics;					}
	BoxPhysics*		getBoxPhysics()			{ return &boxPhysics;				}
	SpriteManager*	getSpriteManager()		{ return spriteManager;				}
	World*			getWorld()				{ return &world;					}
	wstring			getCurrentLevelName()	{ return levelNames[currentLevel];	}

	// METHODS FOR TESTING THE CURRENT GAME STATE
	bool			isAppActive();
	bool			isAtSplashScreen();
	bool			isGameInProgress();
	bool			isGameLevelLoading();
	bool			isWorldRenderable();

	// METHODS FOR TRANSITIONING TO OTHER GAME STATES
	void			goToGame();
	void			goToLoadLevel();
	void			goToMainMenu();

	// METHODS DEFINED in GameStateManager.cpp
	GameStateManager();
	~GameStateManager();
	void			addGameRenderItemsToRenderList(Game *game);
	void			addLevel(wstring levelToAddName, wstring levelToAddFileNameAndPath);
	unsigned int	getLevelNum(wstring levelName);
	void			shutdown();
	void			loadLevel(Game *game, unsigned int levelNum);
	void			loadLevel(Game *game, wstring levelName);
	void			unloadCurrentLevel();
	void			update(Game *game);

	/*Convertsion Methods between the screen coordinate space and the physics
		world coordinate space. I placed this here for now because the game
		state manager manages the updates for both the static world
		(The World class) and the dynamic world (Physics class)*/

	/* Return the center X coordinate (not screen top left)*/
	float32 physicsToScreenX	(float32 physicsX){ return physicsX * WORLD_CONV_FACTOR;}
	/* Return the center Y coordinate (not screen top left)*/
	float32 physicsToScreenY	(float32 physicsY)
		{ return (-physicsY + world.getWorldHeightMeters()) * WORLD_CONV_FACTOR;}
	/* Return the center screen X coordinate*/
	float32 screenToPhysicsX	(float32 screenX) { return screenX/WORLD_CONV_FACTOR;}
	/* Return the center Y screen coordinate*/
	//check this function for correctness later, could be wrong
	float32 screenToPhysicsY	(float32 screenY) 
		{ return (screenY - world.getWorldHeightMeters())/WORLD_CONV_FACTOR;}
};