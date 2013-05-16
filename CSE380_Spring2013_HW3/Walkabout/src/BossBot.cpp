/*
	A Seed is thrown by the player to damage bots.
*/

#include "stdafx.h"
#include "src\Seed.h"
#include "BossBot.h"
#include "src\game\Game.h"
#include "src\gsm\sprite\SpriteManager.h"
#include "src\WalkaboutGame.h"
#include "src\audio\GameAudioManager.h"


BossBot::BossBot(int attSpeed, int attRange, int attDmg, int cooldown, int designation) { 
	attackSpeed = attSpeed; 
	attackRange = attRange; 
	attackDamage = attDmg; 
	attackCooldown = cooldown; 
	cooldownCounter = 0; 
	this->designation = designation; 
	stunned = false;
	direction = 1;
}


void BossBot::update(Game *game){
	////////////////////////
	///////////////////////
	///////////////////////
	if(dead) {
		return;
	}
	// If hitpoints are 0, remove it
	if(hitPoints <= 0){
		setCurrentState(direction==1?DYING_RIGHT:DYING_LEFT);
		game->getGSM()->getSpriteManager()->addBotToRemovalList(this, 15);
		dead = true;
		game->getGSM()->goToLevelWon();
		return;
	}

	// Decrement frames since last attack
	cooldownCounter--;
	if(getCurrentState()==ATTACKING_RIGHT||getCurrentState()==ATTACKING_LEFT){
		if(this->getFrameIndex()==10)
			setCurrentState(direction==1?IDLE_RIGHT:IDLE_LEFT);
	}
	// If can attack, check if player in range.
	if(cooldownCounter <= 0){
		// If player is next to this bot, do something different
		int botX = getCurrentBodyX() * BOX2D_CONVERSION_FACTOR;
		int pX = game->getGSM()->getSpriteManager()->getPlayer()->getCurrentBodyX() * BOX2D_CONVERSION_FACTOR;

		// If the player is within the bots targeting area, go after the player
		if(isInBounds(pX)) {
			int botY = getCurrentBodyY() * BOX2D_CONVERSION_FACTOR;
			int pY = game->getGSM()->getSpriteManager()->getPlayer()->getCurrentBodyY() * BOX2D_CONVERSION_FACTOR;
			// Make sure the player is in the same y area
			if(std::abs(botY - pY) < 200){
				if (pX<botX)
					direction=-1;
				else 
					direction=1;
				cooldownCounter = attackCooldown;
				this->setCurrentState(direction==1?ATTACKING_RIGHT:ATTACKING_LEFT);
				// Seed
				AnimatedSpriteType *seedSpriteType = game->getGSM()->getSpriteManager()->getSpriteType(3);
				Seed *seed = new Seed(PROJECTILE_DESIGNATION, true);

				seed->setHitPoints(1);
				seed->setDamage(SEED_DAMAGE);
				seed->setSpriteType(seedSpriteType);
				seed->setAlpha(255);
				seed->setCurrentState(IDLE_LEFT);
				PhysicalProperties *seedProps = seed->getPhysicalProperties();
				seedProps->setX(botX);
				seedProps->setY(game->getGSM()->getWorld()->getWorldHeight() - botY);
				seedProps->setVelocity(0.0f, 0.0f);
				seedProps->setAccelerationX(0);
				seedProps->setAccelerationY(0);
				seed->setOnTileThisFrame(false);
				seed->setOnTileLastFrame(false);
				seed->affixTightAABBBoundingVolume();

				//create a physics object for the seed
				game->getGSM()->getBoxPhysics()->getPhysicsFactory()->createEnemyObject(game,seed,false);

				float difX = botX - pX;
				float difY = botY - pY;
				// Set the velocity of the seed
				float length = std::sqrt( (difX * difX) + (difY * difY) );

				// Normalize the distances
				difX /= length;
				difY /= length;

				// Scale distances to be x and y velocity
				difX *= PROJECTILE_VELOCITY;
				difY = difY*PROJECTILE_VELOCITY-10;
				seed->getPhysicsBody()->SetLinearVelocity(b2Vec2(-difX, -difY));

				game->getGSM()->getPhysics()->addCollidableObject(seed);
				game->getGSM()->getSpriteManager()->addBot(seed);
			}
		}
	}
	
	
	
	
	
	
	//////////////////////
	///////////////////////
	///////////////////////
	/*
	if(dead) {
		return;
	}
	// If hitpoints are 0, remove it
	if(hitPoints <= 0){
		game->getGSM()->goToLevelWon();
		game->getGSM()->getSpriteManager()->addBotToRemovalList(this, 0);
		dead = true;
		return;
	}

	// Decrement frames since last attack
	cooldownCounter--;
	// If can attack, check if player in range.
	if(cooldownCounter <= 0){
		// If player is next to this bot, do something different
		int botX = getCurrentBodyX() * BOX2D_CONVERSION_FACTOR;
		int pX = game->getGSM()->getSpriteManager()->getPlayer()->getCurrentBodyX() * BOX2D_CONVERSION_FACTOR;

		// If the player is within the bots targeting area, go after the player
		if(isInBounds(pX)) {
			int botY = getCurrentBodyY() * BOX2D_CONVERSION_FACTOR;
			int pY = game->getGSM()->getSpriteManager()->getPlayer()->getCurrentBodyY() * BOX2D_CONVERSION_FACTOR;
			// Make sure the player is in the same y area
			if(std::abs(botY - pY) < 200){
				cooldownCounter = attackCooldown;

				// Seed
				AnimatedSpriteType *seedSpriteType = game->getGSM()->getSpriteManager()->getSpriteType(3);
				Seed *seed = new Seed(PROJECTILE_DESIGNATION, true);
				seed->setHitPoints(1);
				seed->setDamage(SEED_DAMAGE);
				seed->setSpriteType(seedSpriteType);
				seed->setAlpha(255);
				seed->setCurrentState(IDLE_LEFT);
				PhysicalProperties *seedProps = seed->getPhysicalProperties();
				seedProps->setX(botX);
				seedProps->setY(game->getGSM()->getWorld()->getWorldHeight() - botY);
				seedProps->setVelocity(0.0f, 0.0f);
				seedProps->setAccelerationX(0);
				seedProps->setAccelerationY(0);
				seed->setOnTileThisFrame(false);
				seed->setOnTileLastFrame(false);
				seed->affixTightAABBBoundingVolume();

				//create a physics object for the seed
				game->getGSM()->getBoxPhysics()->getPhysicsFactory()->createEnemyObject(game,seed,true);

				// Set the velocity of the seed
				seed->getPhysicsBody()->SetLinearVelocity(b2Vec2(attackSpeed, 0.5));

				game->getGSM()->getPhysics()->addCollidableObject(seed);
				game->getGSM()->getSpriteManager()->addBot(seed);
			}
		}
	}
	*/
}


bool BossBot::isInBounds(int x){
	int bX = getCurrentBodyX() * BOX2D_CONVERSION_FACTOR;

	if(x > (bX + attackRange))
		return false;
	if(x < (bX - attackRange))
		return false;
	return true;
}

void BossBot::playSound(Game *game, SpriteDesignations soundType) {
	if(soundType == SPRITE_DEAD && !dead){
		game->getGAM()->playSound(C_EXPLOSION2);
	}

	if(soundType == SPRITE_HIT){
		game->getGAM()->playSound(C_HIT);
	}
}