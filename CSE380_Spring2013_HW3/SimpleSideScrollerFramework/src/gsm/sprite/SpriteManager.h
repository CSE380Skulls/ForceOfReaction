/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	SpriteManager.h

	This class manages all of the sprites in a given game level. Note
	that the player sprite is also managed by this class.
*/

#pragma once
#include "stdafx.h"
#include "src\graphics\GameGraphics.h"
#include "src\gsm\ai\Bot.h"
#include "src\gsm\ai\BotRecycler.h"
#include "src\gsm\sprite\AnimatedSprite.h"
#include "src\gsm\sprite\AnimatedSpriteType.h"

// Together these two structs will form the bot removal list
struct Node {
	Node* next;
	Bot* spriteToRemove;
	int framesUntilRemoval;
};
struct LinkedList {
	Node *head;
};

class SpriteManager
{
private:
	// NOTE THAT MULTIPLE SPRITES MAY SHARE ARTWORK, SO SPRITE TYPES
	// SPECIFIES A TYPE OF SPRITE, OF WHICH THERE MAY BE MANY INSTANCES
	vector<AnimatedSpriteType*> spriteTypes;

	// THESE ARE THE BOTS IN THE GAME, LIKE ENEMIES, ROCKETS, OR ANYTHING
	// THAT MOVES AROUND AND IS NOT THE PLAYER
	list<Bot*> bots;

	// AND THIS IS THE PLAYER. AS-IS, WE ONLY ALLOW FOR ONE PLAYER AT A TIME
	AnimatedSprite player;

	// THE BotRecycler MAKES SURE WE DON'T HAVE TO CONSTRUCT BOTS WHENEVER
	// WE NEED TO SPAWN THEM, INSTEAD IT WILL RECYCLE THEM FOR US
	BotRecycler recyclableBots;

	// Remove bots from the game
	LinkedList botRemovalList;

	// Update the bot removal list, decrement counters and remove bots if necessarry
	void                updateBotRemovalList();

public:
	// NOTHING TO INIT OR DESTROY
	SpriteManager()		{ botRemovalList.head = NULL; }
	~SpriteManager()	{}

	// INLINED ACCESSOR METHODS
	int						getNumberOfSprites()	{ return bots.size();		}
	AnimatedSprite*			getPlayer()				{ return &player;			}
	list<Bot*>::iterator	getBotsIterator()		{ return bots.begin();		}
	list<Bot*>::iterator	getEndOfBotsIterator()	{ return bots.end();		}

	// METHODS DEFINED IN SpriteManager.cpp
	void				addBot(Bot *botToAdd);
	void				addSpriteItemsToRenderList(Game *game);
	unsigned int		addSpriteType(AnimatedSpriteType *spriteTypeToAdd);
	void				addSpriteToRenderList(Game *game, AnimatedSprite *sprite, RenderList *renderList, Viewport *viewport);
	void				clearSprites();
	AnimatedSpriteType* getSpriteType(unsigned int typeIndex);
	void				removeBot(Bot *botToRemove);
	void				unloadSprites();
	void				unloadAnimatedSprites();
	void				update(Game *game);
	void                addBotToRemovalList(Bot* sprite, int framesUntilRemoval);

	//Box2d test method: this will update all of the sprite locations for rendering
	void				updateSpriteLocations(int conversion_factor);
};