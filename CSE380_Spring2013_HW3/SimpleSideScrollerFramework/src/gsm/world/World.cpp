/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	World.cpp

	See World.h for a class description.
*/

#include "stdafx.h"
#include "src\dataLoader\GameDataLoader.h"
#include "src\game\Game.h"
#include "src\graphics\GameGraphics.h"
#include "src\graphics\RenderList.h"
#include "src\graphics\TextureManager.h"
#include "src\gsm\sprite\SpriteManager.h"
#include "src\gsm\state\GameStateManager.h"
#include "src\gsm\world\World.h"
#include "src\gui\Viewport.h"

/*
	World - Default Constructor, it constructs the layers
	vector, allowing new layers to be added.
*/
World::World()	
{
	layers = new vector<WorldLayer*>();
	worldWidth = 0;
	worldHeight = 0;
}

/*
	~World - This destructor will remove the memory allocated
	for the layer vector.
*/
World::~World()	
{
	delete layers;
}

/*
	addLayer - This method is how layers are added to the World.
	These layers might be TiledLayers, SparseLayers, or 
	IsometricLayers, all of which are child classes of WorldLayer.
*/
void World::addLayer(WorldLayer *layerToAdd)
{
	layers->push_back(layerToAdd);
}

/*
	addWorldRenderItemsToRenderList - This method sends the render
	list and viewport to each of the layers such that they
	may fill it with RenderItems to draw.
*/
void World::addWorldRenderItemsToRenderList(Game *game)
{
	GameStateManager *gsm = game->getGSM();
	GameGUI *gui = game->getGUI();
	if (gsm->isWorldRenderable())
	{
		GameGraphics *graphics = game->getGraphics();
		RenderList *renderList = graphics->getWorldRenderList();
		Viewport *viewport = gui->getViewport();
		for (unsigned int i = 0; i < layers->size(); i++)
		{
			layers->at(i)->addRenderItemsToRenderList(	renderList,
														viewport);
		}
	}
}

void World::initWorldPhysicsSystem(Game *game){
	vector<WorldLayer*>::iterator layerIt = layers->begin();
	while(layerIt != layers->end()){
		WorldLayer * layer = (*layerIt);
		layer->addItemsToPhysicsSystem(game);
		layerIt++;
	}
}

/*
	clear - This method removes all data from the World. It should
	be called first when a level is unloaded or changed. If it
	is not called, an application runs the risk of having lots
	of extra data sitting around that may slow the progam down.
	Or, if the world thinks a level is still active, it might add
	items to the render list using image ids that have already been
	cleared from the GameGraphics' texture manager for the world.
	That would likely result in an exception.
*/
void World::unloadWorld()
{
	// A list to add things to to then remove them, weird errors removing from vector
	list<WorldLayer*> myList;

	for(int x = 0; x < layers->size(); x++){
		myList.push_front(layers->at(x));
	}
	layers->clear();

	list<WorldLayer*>::iterator listIterator = myList.begin();

	while(listIterator != myList.end()){
		TiledLayer *temp = (TiledLayer*)(*listIterator);
		listIterator++;
		myList.remove(temp);
		delete temp;
	}

	//layers->clear();
	worldWidth = 0;
	worldHeight = 0;
}

/*
	update - This method should be called once per frame. Note that
	the World is for static objects, so we don't have anything
	to update as/is. But, should the need arise, one could add non-
	collidable layers to a game world and update them here. For
	example, particle systems.
*/
void World::update(Game *game)
{
	// NOTE THAT THIS METHOD IS NOT IMPLEMENTED BUT COULD BE
	// SHOULD YOU WISH TO ADD ANY NON-COLLIDABLE LAYERS WITH
	// DYNAMIC CONTENT OR PARTICLE SYSTEMS
}