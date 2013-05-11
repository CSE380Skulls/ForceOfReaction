/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	AnimatedSprite.h

	This class represents a sprite that can can
	be used to animate a game character or object.
*/

#pragma once
#include "stdafx.h"
#include "src\gsm\physics\CollidableObject.h"
#include "src\game\Game.h"
#include "src\gsm\physics\BoxPhysicsObject.h"
#include "src\gsm\physics\PhysicalProperties.h"
#include "src\gsm\sprite\AnimatedSpriteType.h"
#include "src\gui\Viewport.h"
#include "src\gsm\sprite\SpriteDesignations.h"

class AnimatedSprite : public CollidableObject , public BoxPhysicsObject
{
protected:
	// SPRITE TYPE FOR THIS SPRITE. THE SPRITE TYPE IS THE ID
	// OF AN AnimatedSpriteType OBJECT AS STORED IN THE SpriteManager
	AnimatedSpriteType *spriteType;

	// TRANSPARENCY/OPACITY
	int alpha;

	// THE "current" STATE DICTATES WHICH ANIMATION SEQUENCE 
	// IS CURRENTLY IN USE, BUT IT MAP ALSO BE USED TO HELP
	// WITH OTHER GAME ACTIVITIES, LIKE PHYSICS
	wstring currentState;

	// THE INDEX OF THE CURRENT FRAME IN THE ANIMATION SEQUENCE
	// NOTE THAT WE WILL COUNT BY 2s FOR THIS SINCE THE VECTOR
	// THAT STORES THIS DATA HAS (ID,DURATION) TUPLES
	unsigned int frameIndex;

	// USED TO ITERATE THROUGH THE CURRENT ANIMATION SEQUENCE
	unsigned int animationCounter;
	// All animated sprites have hitpoints
	int hitPoints;
	// Is this animated sprite dead?
	bool dead;
	// How much damage does this sprite do if it hits something?
	int damage;
	// What type of sprite is this
	int designation;
	// Can this sprite move/attack?
	bool stunned;

	// THIS IS CURRENTLY ONLY USED FOR SEEDS STICKING TO WALLS, MAY BE A BETTER WAY...
	bool wallCollision;

public:
	// INLINED ACCESSOR METHODS
	int					getAlpha()			{ return alpha;				}
	wstring				getCurrentState()	{ return currentState;		}
	unsigned int		getFrameIndex()		{ return frameIndex;		}
	AnimatedSpriteType*	getSpriteType()		{ return spriteType;		}

	// INLINED MUTATOR METHODS
	void setAlpha(int initAlpha)
	{	alpha = initAlpha;						}
	void setSpriteType(AnimatedSpriteType *initSpriteType)
	{	spriteType = initSpriteType;}
	void setHitPoints(int hp)
	{   hitPoints = hp; }
	void setDamage(int dmg){
		damage = dmg;
	}

	// METHODS DEFINED IN AnimatedSprite.cpp
	AnimatedSprite();
	~AnimatedSprite();
	void changeFrame();
	unsigned int getCurrentImageID();
	void setCurrentState(wstring newState);
	void updateSprite();
	void affixTightAABBBoundingVolume();
	void correctToTightBoundingVolume();
	void decrementHitPoints(int damage) { hitPoints -= damage; }
	int getHitPoints() { return hitPoints; }
	int getDamage() { return damage; }
	int getDesignation() { return designation; }
	void setWallCollision() { wallCollision = true; }
	virtual void update(Game *game)=0;
	virtual void playSound(Game *game, SpriteDesignations soundType)=0;
	virtual void stun(int framesStunned)=0;
};