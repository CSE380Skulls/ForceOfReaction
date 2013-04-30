/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	World.h

	This class manages the static data for the game level being
	played. This means all game backgrounds, which are stored and
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
#include "src\gsm\world\WorldLayer.h"

class Game;
class SpriteManager;

/*
	This is the EXTREMELY important conversion factor between the screen
	space and the world space. 

	TODO: This may not be the place for this, but I will leave it here for
		now to conduct some tests on Box2D
*/

//static const float32 WORLD_CONV_FACTOR = 32.0f;

class World
{
private:
	// NOTE, THE worldWidth & worldHeight REPRESENT THE FULL
	// SIZE OF THIS LEVEL. WHICH SHOULD BE AT LEAST THE SIZE OF
	// THE VIEWPORT, AND LIKELY MUCH LARGER, WHICH WOULD THEN
	// REQUIRE SCROLLING TO SEE IT.
	int worldWidth;
	int worldHeight;
	float32 WORLD_CONV_FACTOR;

	// THESE ARE THE BACKGROUND LAYERS
	vector<WorldLayer*> *layers;

public:
	// INLINED ACCESSOR METHODS
	vector<WorldLayer*>*	getLayers()				{ return layers;			}
	int						getNumLayers()			{ return layers->size();	}
	int						getWorldHeight()		{ return worldHeight;		}
	int						getWorldWidth()			{ return worldWidth;		}
	float32					getWorldHeightMeters()	{ return worldHeight/WORLD_CONV_FACTOR; }
	float32					getWorldWidthMeters()	{ return worldWidth/WORLD_CONV_FACTOR; }

	// INLINED MUTATOR METHODS
	void setWorldHeight(int initWorldHeight)
	{ worldHeight = initWorldHeight;		}
	void setWorldWidth(int initWorldWidth)
	{ worldWidth = initWorldWidth;			}

	// METHODS DEFINED in GameStateManager.cpp
	World();
	~World();
	void	addLayer(WorldLayer *layerToAdd);
	void	addWorldRenderItemsToRenderList(Game *game);
	void	initWorldPhysicsSystem(Game *game);
	void	unloadWorld();
	void	update(Game *game);
	void    setWorldConvFactor(float32 convFactor) { WORLD_CONV_FACTOR = convFactor; }
	float32 getWorldConvFactor() { return WORLD_CONV_FACTOR; }
};