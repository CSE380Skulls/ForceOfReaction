/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	SpriteManager.cpp

	See SpriteManager.h for a class description.
*/

#pragma once
#include "stdafx.h"
#include "src\gsm\ai\Bot.h"
#include "src\gsm\physics\PhysicalProperties.h"
#include "src\graphics\GameGraphics.h"
#include "src\gsm\sprite\AnimatedSprite.h"
#include "src\gsm\sprite\AnimatedSpriteType.h"
#include "src\gsm\sprite\SpriteManager.h"
#include "src\gsm\state\GameStateManager.h"

/*
	addSpriteToRenderList - This method checks to see if the sprite
	parameter is inside the viewport. If it is, a RenderItem is generated
	for that sprite and it is added to the render list.
*/
void SpriteManager::addSpriteToRenderList(Game *game, AnimatedSprite *sprite,
										  RenderList *renderList,
										  Viewport *viewport)
{
	// GET THE SPRITE TYPE INFO FOR THIS SPRITE
	AnimatedSpriteType *spriteType = sprite->getSpriteType();
	PhysicalProperties *pp = sprite->getPhysicalProperties();

	//IMPORTANT STEP
	/******* Update the sprite's screen coordinates from physics world ******/
	pp->setX(game->getGSM()->physicsToScreenX(sprite->getCurrentBodyX()) - spriteType->getTextureWidth()/2);
	pp->setY(game->getGSM()->physicsToScreenY(sprite->getCurrentBodyY()) - spriteType->getTextureHeight()/2);

	// IS THE SPRITE VIEWABLE?
	if (viewport->areWorldCoordinatesInViewport(	
									pp->getX(),
									pp->getY(),
									spriteType->getTextureWidth(),
									spriteType->getTextureHeight()))
	{
		// SINCE IT'S VIEWABLE, ADD IT TO THE RENDER LIST
		RenderItem itemToAdd;
		itemToAdd.id = sprite->getFrameIndex();
		renderList->addRenderItem(	sprite->getCurrentImageID(),
									pp->round(pp->getX()-viewport->getViewportX()),
									pp->round(pp->getY()-viewport->getViewportY()),
									pp->round(pp->getZ()),
									sprite->getAlpha(),
									spriteType->getTextureWidth(),
									spriteType->getTextureHeight());
	}
}

/*
	addSpriteItemsToRenderList - This method goes through all of the sprites,
	including the player sprite, and adds the visible ones to the render list.
	This method should be called each frame.
*/
void SpriteManager::addSpriteItemsToRenderList(	Game *game)
{
	GameStateManager *gsm = game->getGSM();
	GameGUI *gui = game->getGUI();
	if (gsm->isWorldRenderable())
	{
		GameGraphics *graphics = game->getGraphics();
		RenderList *renderList = graphics->getWorldRenderList();
		Viewport *viewport = gui->getViewport();

		// ADD THE PLAYER SPRITE
		addSpriteToRenderList(game, player, renderList, viewport);

		// NOW ADD THE REST OF THE SPRITES
		list<Bot*>::iterator botIterator;
		botIterator = bots.begin();
		while (botIterator != bots.end())
		{			
			Bot *bot = (*botIterator);
			addSpriteToRenderList(game, bot, renderList, viewport);
			botIterator++;
		}
	}
}

/*
	addSprite - This method is for adding a new sprite to 
	this sprite manager. Once a sprite is added it can be 
	scheduled for rendering.
*/
void SpriteManager::addBot(Bot *botToAdd)
{
	bots.push_back(botToAdd);
}

/*
	addSpriteType - This method is for adding a new sprite
	type. Note that one sprite type can have many sprites. For
	example, we may say that there may be a "Bunny" type of
	sprite, and specify properties for that type. Then there might
	be 100 different Bunnies each with their own properties, but that
	share many things in common according to what is defined in
	the shared sprite type object.
*/
unsigned int SpriteManager::addSpriteType(AnimatedSpriteType *spriteTypeToAdd)
{
	spriteTypes.push_back(spriteTypeToAdd);
	return spriteTypes.size()-1;
}

/*
	clearSprites - This empties all of the sprites and sprite types.
*/
void SpriteManager::clearSprites()
{
	spriteTypes.clear();
	bots.clear();
}
/*
	getSpriteType - This gets the sprite type object that corresponds
	to the index argument.
*/
AnimatedSpriteType* SpriteManager::getSpriteType(unsigned int typeIndex)
{
	if (typeIndex < spriteTypes.size())
		return spriteTypes.at(typeIndex);
	else
		return NULL;
}

/*
	unloadSprites - This method removes all artwork from memory that
	has been allocated for game sprites.
*/
void SpriteManager::unloadSprites()
{
	/*
	vector<AnimatedSpriteType*>::iterator spriteIterator;
	spriteIterator = spriteTypes.begin();
	// Iterate over all sprites and delete them all.
	while(spriteIterator != spriteTypes.end()){
		AnimatedSpriteType *temp = (*spriteIterator);
		spriteIterator++;
		delete temp;
	}
	*/
}

void SpriteManager::unloadAnimatedSprites()
{
	AnimatedSprite *player = getPlayer();

	// Remove the player from the box2d world.
	player->returnPhysicsBody();

	list<Bot*>::iterator botIterator;
	botIterator = bots.begin();
	// Iterate over all sprites and delete them all.
	while(botIterator != bots.end()){
		Bot *temp = (*botIterator);
		// Remove from box2d
		temp->returnPhysicsBody();
		// Remove from bots list
		bots.remove(*(botIterator++));
		// Free memory
		delete temp;
	}
	// Clear out the bot removal list
	clearBotRemovalList();
}

void SpriteManager::removeBot(Bot *botToRemove)
{
	// Remove bot from list of bots
	bots.remove(botToRemove);
	// Remove bot from box2d
	botToRemove->returnPhysicsBody();
	// Free the memory
	delete botToRemove;
	//return NULL;
}

/*
	update - This method should be called once per frame. It
	goes through all of the sprites, including the player, and calls their
	update method such that they may update themselves.
*/
void SpriteManager::update(Game *game)
{
	// Update the bot removal List
	updateBotRemovalList();

	// UPDATE THE PLAYER SPRITE
	player->updateSprite();
	player->update(game);

	// NOW UPDATE THE REST OF THE SPRITES
	list<Bot*>::iterator botIterator;
	botIterator = bots.begin();
	while (botIterator != bots.end())
	{
		Bot *bot = (*botIterator);
		bot->think(game);
		bot->updateSprite();
		botIterator++;
	}
}

// This adds a bot to the removal list, it decrements its frames untilRemoval value by the values of all of those
// in front of it, therefore you only need to decrement the value of the first node in the list when
// updating this removal list.
// NOTE:: THIS WILL BREAK IF THERE IS A SPRITE WITH A FRAMES UNTIL REMOVAL LESSER THEN THE FRAMES UNTIL REMOVAL
// OF A BOT CURRENTLY IN THE LIST.  AS OF RIGHT NOW I DON'T THINK THIS IS A PROBLEM BECAUSE EVERYTHING WILL
// PROBABLY HAVE THE SAME AMMOUNT OF FRAMES UNTIL REMOVAL BUT THIS METHOD MUST BE CHANGED IF DIFFERENT BOTS
// ARE REMOVED AFTER DIFFERENT INTERVALS OF TIME.
void SpriteManager::addBotToRemovalList(Bot* bot, int framesUntilRemoval){

	// Create the new node to be added to list.
	Node *n = new Node();
	n->framesUntilRemoval = framesUntilRemoval;
	n->spriteToRemove = bot;
	n->next = NULL;

	// If nothing is in the list, make this the first element in the list
	if(botRemovalList.head == NULL){
		botRemovalList.head = n;
	}
	else {
		// Make the head element the next of this element and insert this element at the head.
		n->next = botRemovalList.head;
		botRemovalList.head = n;
	}
}

// Decrement the frames until removal of the first bot in the list and removal all bots that need to be
// removed this frame. This method removes the bots from box2d as well as the render list.
void SpriteManager::updateBotRemovalList(){
	// Special Case for head of list
	if(botRemovalList.head != NULL){
		botRemovalList.head->framesUntilRemoval--;
	}

	// Special Case for head of list
	while(botRemovalList.head != NULL && botRemovalList.head->framesUntilRemoval <= 0){
		Node* temp = botRemovalList.head;
		botRemovalList.head = temp->next;
		if(botRemovalList.head != NULL)
			botRemovalList.head->framesUntilRemoval--;
		
		Bot *b = temp->spriteToRemove;
		delete temp;
		removeBot(b);
	}
	// At this point I'm guarenteed that the head of the list has been decremented and is either null 
	//or doesn't need to be removed.

	// If the head is null then don't need to remove anything else
	if(botRemovalList.head != NULL){
		Node *previous = botRemovalList.head;
		Node *current = botRemovalList.head->next;
		// Iterate through the list and update all of the times until removals.
		while(current != NULL){
			current->framesUntilRemoval--;

			// If this node has to be removed, remove it
			if(current->framesUntilRemoval <= 0){
				Node *temp = current;
				current = current->next;
				previous->next = current;

				Bot *b = temp->spriteToRemove;

				delete temp;
				removeBot(b);
			}
			else {
				// Continue iterating through linked list
				previous = current;
				current = current->next;
			}
		}
	}
}

// Free all the nodes in the bot Removal list
void SpriteManager::clearBotRemovalList() {
	Node *n = botRemovalList.head;
	botRemovalList.head = NULL;

	// Free all of the nodes in the bot Removal List
	while(n != NULL){
		Node *temp = n;
		n = n->next;
		delete temp;
	}
}

///* This will update all of the sprite rendering information after the box2d
//	physics step has happened. This function may be irelevant for where it is
//	being used now (in the boxphysics update function). I would perfer that this
//	conversion happens right before the renderlist item is placed in the render
//	list. (seems appropriate)*/
//
//void SpriteManager::updateSpriteLocations(int conversion_factor){
//	
//}