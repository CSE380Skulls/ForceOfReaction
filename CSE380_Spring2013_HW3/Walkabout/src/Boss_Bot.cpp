/*
	A Seed is thrown by the player to damage bots.
*/

#include "stdafx.h"
#include "src\Seed.h"
#include "Boss_Bot.h"
#include "src\game\Game.h"
#include "src\gsm\sprite\SpriteManager.h"
#include "src\WalkaboutGame.h"
#include "src\audio\GameAudioManager.h"


Boss_Bot::Boss_Bot(int att_speed, int att_range, int att_dmg, int cool_down, int designation) { 
	attack_Speed = att_speed; 
	attack_Range = att_range; 
	attack_Damage = att_dmg; 
	attack_Cool_Down = cool_down; 
	cd_Counter = 0; 
	this->designation = designation; 
	stunned = false;
}


void Boss_Bot::update(Game *game){
	if(dead)
		return;
	// If hitpoints are 0, remove it
	if(hitPoints <= 0){
		game->getGSM()->getSpriteManager()->addBotToRemovalList(this, 0);
		dead = true;
		return;
	}

	// Decrement frames since last attack
	cd_Counter--;
	// If can attack, check if player in range.
	if(cd_Counter <= 0){
		// If player is next to this bot, do something different
		int botX = getCurrentBodyX() * BOX2D_CONVERSION_FACTOR;
		int pX = game->getGSM()->getSpriteManager()->getPlayer()->getCurrentBodyX() * BOX2D_CONVERSION_FACTOR;

		// If the player is within the bots targeting area, go after the player
		if(isInBounds(pX)) {
			int botY = getCurrentBodyY() * BOX2D_CONVERSION_FACTOR;
			int pY = game->getGSM()->getSpriteManager()->getPlayer()->getCurrentBodyY() * BOX2D_CONVERSION_FACTOR;
			// Make sure the player is in the same y area
			if(std::abs(botY - pY) < 200){
				cd_Counter = attack_Cool_Down;

				// Seed
				AnimatedSpriteType *seedSpriteType = game->getGSM()->getSpriteManager()->getSpriteType(3);
				Seed *seed = new Seed(PROJECTILE_DESIGNATION);
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

				// Set the velocity of the seed
				seed->getPhysicsBody()->SetLinearVelocity(b2Vec2(attack_Speed, 0.5));

				game->getGSM()->getPhysics()->addCollidableObject(seed);
				game->getGSM()->getSpriteManager()->addBot(seed);
			}
		}
	}
}


bool Boss_Bot::isInBounds(int x){
	int bX = getCurrentBodyX() * BOX2D_CONVERSION_FACTOR;

	if(x > (bX + attack_Range))
		return false;
	if(x < (bX - attack_Range))
		return false;
	return true;
}

void Boss_Bot::playSound(Game *game, SpriteDesignations soundType) {
	if(soundType == SPRITE_DEAD && !dead){
		game->getGAM()->playSound(C_EXPLOSION2);
	}

	if(soundType == SPRITE_HIT){
		game->getGAM()->playSound(C_HIT);
	}
}