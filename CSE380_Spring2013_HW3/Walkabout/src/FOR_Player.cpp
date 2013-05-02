
#include "stdafx.h"
#include "src\Seed.h"
#include "src\Vine.h"
#include "src\game\Game.h"
#include "src\gsm\sprite\SpriteManager.h"
#include "src\gui\ScreenGUI.h"
#include "src\WalkaboutGame.h"
#include "FOR_Player.h"
#include "src\gsm\sprite\SpriteDesignations.h"
#include "src\audio\GameAudioManager.h"
#include "src\FireBall.h"


// Initialize stuff, att_Cooldown -> The number of frames between player attacks
FOR_Player::FOR_Player(int att_Cooldown, int d_Cooldown, int designation){
	dead = false;
	stunned = false;
	//isInvincible = false;
	cd_Counter = 0;
	death_Cooldown = d_Cooldown;
	attack_Cooldown = att_Cooldown;
	selected_element = NOTHING;
	this->designation = designation;
	available_Elements = 3;
}

// Can this player attack?  Assumes that the player is going to attack if they can
bool FOR_Player::canAttack(){
	if(stunned)
		return false;
	if(cd_Counter <= 0)
		return true;
	return false;
}

void FOR_Player::update(Game* game){
	updateStatusGUI(game);
	cd_Counter--;
	// If dead and cd_Counter <= 0
	if(dead) {
		if(cd_Counter <= 0)
			game->getGSM()->goToGameOver();
		return;
	}
	// If stunned and shouldn't be anymore, unstun
	if(stunned && (cd_Counter <= 0) )
		stunned = false;

	if(isInvincible && (cd_Counter <= 0))
		isInvincible = false;

	// If hitpoints are 0, remove it
	if(hitPoints <= 0){
		dead = true;
		cd_Counter = death_Cooldown;
		setCurrentState(DEAD);
		return;
	}
	// Quick fix for winning a level
	if(game->getGSM()->getSpriteManager()->areEnemiesDead())
		game->getGSM()->goToLevelWon();
}

void FOR_Player::updateStatusGUI(Game* game){
	ScreenGUI *gui = game->getGUI()->getScreen(GS_GAME_IN_PROGRESS);
	TextureManager *tm = game->getGraphics()->getGUITextureManager();
	int hp = this->getHitPoints()/(PLAYER_HITPOINTS/10);
	if (this->getHitPoints()<0)
		hp = 0;
	gui->popOverlayImage(4);

	unsigned int imageID;
	OverlayImage *imageToAdd = new OverlayImage();
	switch(hp)
	{
	case 0:
		imageID = tm->retrieveTexture(W_STATUS_HP_0_PATH);
		break;
	case 1:
		imageID = tm->retrieveTexture(W_STATUS_HP_1_PATH);
		break;
	case 2:
		imageID = tm->retrieveTexture(W_STATUS_HP_2_PATH);
		break;
	case 3:
		imageID = tm->retrieveTexture(W_STATUS_HP_3_PATH);
		break;
	case 4:
		imageID = tm->retrieveTexture(W_STATUS_HP_4_PATH);
		break;
	case 5:
		imageID = tm->retrieveTexture(W_STATUS_HP_5_PATH);
		break;
	case 6:
		imageID = tm->retrieveTexture(W_STATUS_HP_6_PATH);
		break;
	case 7:
		imageID = tm->retrieveTexture(W_STATUS_HP_7_PATH);
		break;
	case 8:
		imageID = tm->retrieveTexture(W_STATUS_HP_8_PATH);
		break;
	case 9:
		imageID = tm->retrieveTexture(W_STATUS_HP_9_PATH);
		break;
	case 10:
		imageID = tm->retrieveTexture(W_STATUS_HP_10_PATH);
		break;
	}
	imageToAdd->x = 84;
	imageToAdd->y = 15;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 320;
	imageToAdd->height = 22;
	imageToAdd->imageID = imageID;
	gui->addOverlayImage(imageToAdd);

	imageToAdd = new OverlayImage();
	if (selected_element == EARTH)
		imageID = tm->retrieveTexture(W_STATUS_EARTH_SELECTED_PATH);
	else 
		imageID = tm->retrieveTexture(W_STATUS_EARTH_UNSELECTED_PATH);
	imageToAdd->x = 110;
	imageToAdd->y = 50;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 48;
	imageToAdd->height = 48;
	imageToAdd->imageID = imageID;
	gui->addOverlayImage(imageToAdd);

	imageToAdd = new OverlayImage();
	if (selected_element == WATER)
		imageID = tm->retrieveTexture(W_STATUS_WATER_SELECTED_PATH);
	else 
		imageID = tm->retrieveTexture(W_STATUS_WATER_UNSELECTED_PATH);
	imageToAdd->x = 200;
	imageToAdd->y = 50;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 48;
	imageToAdd->height = 48;
	imageToAdd->imageID = imageID;
	gui->addOverlayImage(imageToAdd);

	imageToAdd = new OverlayImage();
	if (selected_element == FIRE)
		imageID = tm->retrieveTexture(W_STATUS_FIRE_SELECTED_PATH);
	else 
		imageID = tm->retrieveTexture(W_STATUS_FIRE_UNSELECTED_PATH);
	imageToAdd->x = 290;
	imageToAdd->y = 50;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 48;
	imageToAdd->height = 48;
	imageToAdd->imageID = imageID;
	gui->addOverlayImage(imageToAdd);
}

void FOR_Player::playSound(Game* game, SpriteDesignations soundType){

	if(soundType == SPRITE_DEAD && !dead){
		game->getGAM()->playSound(C_DEATH);
	}

	if(soundType == SPRITE_HIT){
		game->getGAM()->playSound(C_PLAYERHIT);
	}
}

void FOR_Player::stun(int framesStunned) {
	// If the player is dead, don't do anything if gets stunned
	if(!dead && !stunned) {
		stunned = true;
		cd_Counter =  framesStunned;
	}
}

//rotate selected element.
//THIS CURRENTLY causes a problem where the first element you select is water:
//this is a side effect of starting at element NOTHING when you have 3 available elements.
//this will not happen if you have element NOTHING and have NO available elements.
void FOR_Player::nextElement() {
	int element = available_Elements>0?(getSelectedElement()+1)%available_Elements:NOTHING;
	setSelectedElement(element);
}

void FOR_Player::run() {
	if(canMove()) {
		getPhysicsBody()->SetLinearVelocity(
				b2Vec2(	PLAYER_VELOCITY*direction,
						getPhysicsBody()->GetLinearVelocity().y));

		if (getPhysicsBody()->GetLinearVelocity().y==0)
			setCurrentState(direction==1?WALKING_RIGHT:WALKING_LEFT);
		else if(isAttacking() && getFrameIndex()==12)
			setCurrentState(direction==1?FALLING_RIGHT:FALLING_LEFT);
	}
}

void FOR_Player::hover() {
	getPhysicsBody()->SetLinearVelocity(b2Vec2(0, getPhysicsBody()->GetLinearVelocity().y));
	if(		isMovingV() && !(isFloating()||isAttacking()) ||
			(isAttacking() && getFrameIndex()==12))
		setCurrentState(direction==1?FALLING_RIGHT:FALLING_LEFT);
	if(!isMovingV() && !isAttacking())
		setCurrentState(direction==1?IDLE_RIGHT:IDLE_LEFT);
	if(!isMovingV() && isAttacking() && getFrameIndex()==12)
		setCurrentState(direction==1?IDLE_RIGHT:IDLE_LEFT);
}

void FOR_Player::jump(Game *game){
	if(canMove()) {
		if(getPhysicsBody()->GetLinearVelocity().y==0)
		{
			game->getGAM()->playSound(C_JUMP);
			getPhysicsBody()->ApplyLinearImpulse(
					b2Vec2(0.0f, JUMP_VELOCITY),
					getPhysicsBody()->GetPosition());
			setCurrentState(direction==1?JUMPING_RIGHT:JUMPING_LEFT);
		}
	}
}

void FOR_Player::leftAttack(Game* game, float mx, float my){
	if(canAttack()){
		if(getSelectedElement() == EARTH)
			earthAttackL(game, mx, my);
		if(getSelectedElement() == WATER)
			waterAttackL(game, mx, my);
		if(getSelectedElement() ==FIRE)
			fireAttackL(game, mx, my);

		// Set the attack cooldown, just attacked
		cd_Counter = attack_Cooldown;
	}
}

void FOR_Player::rightAttack(Game *game, float mx, float my){
	if(canAttack()) {
		if(getSelectedElement() == EARTH)
			earthAttackR(game, mx, my);
		if(getSelectedElement() == WATER)
			waterAttackR(game, mx, my);
		if(getSelectedElement() ==FIRE)
			fireAttackR(game, mx, my);
		// Set the attack cooldown, just attacked
		cd_Counter = attack_Cooldown;
	}
}

void FOR_Player::earthAttackL(Game *game, float mx, float my) {
	game->getGAM()->playSound(C_SWING);
	setCurrentState(direction==1?ATTACKING_RIGHT:ATTACKING_LEFT);

	float px = game->getGSM()->physicsToScreenX(getCurrentBodyX());
	float py = game->getGSM()->physicsToScreenY(getCurrentBodyY());
	//py = game->getGSM()->getWorld()->getWorldHeight() - py;

	// Get the difference bewteen these loactions
	float difX = mx - px;
	float difY = my - py;
	
	float vine_x;
	if(difX > 0){
		setDirection(1);
		setCurrentState(ATTACKING_RIGHT);
		vine_x = px + game->getGSM()->getSpriteManager()->getPlayer()->getSpriteType()->getTextureWidth() / 2.0;
	}
	else {
		setDirection(-1);
		setCurrentState(ATTACKING_LEFT);
		vine_x = px - game->getGSM()->getSpriteManager()->getPlayer()->getSpriteType()->getTextureWidth() / 2.0;
	}

	// Get the length of the vector from player to mouse
	float length = std::sqrt( (difX * difX) + (difY * difY) );

	// Normalize the distances
	difX /= length;
	difY /= length;

	// Scale distances to be x and y velocity
	difX *= PROJECTILE_VELOCITY *4;
	difY *= PROJECTILE_VELOCITY *4;

	float box_player_x = game->getGSM()->screenToPhysicsX(vine_x);
	float box_player_y = game->getGSM()->screenToPhysicsY(py);

	//Create TEST ROPE!
	vector<AnimatedSprite *> spritesList;
	AnimatedSpriteType *vineSpriteType = game->getGSM()->getSpriteManager()->getSpriteType(4);
	SpriteManager *spriteManager = game->getGSM()->getSpriteManager();
	for(int i = 0; i < 10; i++){
		Vine *vine = new Vine(PROJECTILE_DESIGNATION);
		vine->setHitPoints(1);
		vine->setDamage(VINE_DAMAGE);
		vine->setSpriteType(vineSpriteType);
		vine->setAlpha(255);
		vine->setCurrentState(IDLE_LEFT);
		PhysicalProperties *vineProps = vine->getPhysicalProperties();
		vineProps->setX(px);
		vineProps->setY(py);
		vineProps->setVelocity(0.0f, 0.0f);
		vineProps->setAccelerationX(0);
		vineProps->setAccelerationY(0);
		vine->setOnTileThisFrame(false);
		vine->setOnTileLastFrame(false);
		vine->affixTightAABBBoundingVolume();
		spriteManager->addAuxiliarySprite(vine);
		spritesList.push_back(vine);
	}
	//now create the test rope
	game->getGSM()->getBoxPhysics()->getPhysicsFactory()->createAttackRope(game,spritesList,
		box_player_x,box_player_y);

	//set the velocity for the first segment of the vine
	spritesList[0]->getPhysicsBody()->SetLinearVelocity(b2Vec2(difX, -difY));

	//float vineX;
	//if(mx > px){
	//	// Right side click
	//	setDirection(1);
	//	setCurrentState(ATTACKING_RIGHT);
	//	vineX = px + game->getGSM()->getSpriteManager()->getPlayer()->getSpriteType()->getTextureWidth() / 2.0;
	//}
	//else {
	//	// Left side click
	//	setDirection(-1);
	//	setCurrentState(ATTACKING_LEFT);
	//	vineX = px - vineSpriteType->getTextureWidth()*2;
	//}

	//Vine *vine = new Vine(PROJECTILE_DESIGNATION);
	//vine->init(vineX,py,vineSpriteType);

	////create a physics object for the vine
	//game->getGSM()->getBoxPhysics()->getPhysicsFactory()->createPlayerObject(game,vine,false);
	//game->getGSM()->getPhysics()->addCollidableObject(vine);
	//game->getGSM()->getSpriteManager()->addBot(vine);

	// Remove this vine after 15 frames
	//game->getGSM()->getSpriteManager()->addBotToRemovalList(vine, 15);
}

// Throw a seed
void FOR_Player::earthAttackR(Game *game, float mx, float my){
	game->getGAM()->playSound(C_SWING);
	// Get mouse and player locations
	float px = game->getGSM()->physicsToScreenX(getCurrentBodyX());
	float py = game->getGSM()->physicsToScreenY(getCurrentBodyY());
	//py = game->getGSM()->getWorld()->getWorldHeight() - py;

	// Get the difference bewteen these loactions
	float difX = mx - px;
	float difY = my - py;

	if(difX > 0){
		setDirection(1);
		setCurrentState(ATTACKING_RIGHT);
	}
	else {
		setDirection(-1);
		setCurrentState(ATTACKING_LEFT);
	}

	// Get the length of the vector from player to mouse
	float length = std::sqrt( (difX * difX) + (difY * difY) );

	// Normalize the distances
	difX /= length;
	difY /= length;

	// Scale distances to be x and y velocity
	difX *= PROJECTILE_VELOCITY;
	difY *= PROJECTILE_VELOCITY;

	// Seed
	AnimatedSpriteType *seedSpriteType = game->getGSM()->getSpriteManager()->getSpriteType(3);
	Seed *seed = new Seed(PROJECTILE_DESIGNATION);
	seed->init(px,py,seedSpriteType);

	//create a physics object for the seed
	game->getGSM()->getBoxPhysics()->getPhysicsFactory()->createFriendlyProjectile(game,seed,true);
	game->getGSM()->getPhysics()->addCollidableObject(seed);
	game->getGSM()->getSpriteManager()->addBot(seed);

	// Set the velocity of the seed
	seed->getPhysicsBody()->SetLinearVelocity(b2Vec2(difX, -difY));
}

void FOR_Player::waterAttackL(Game *game, float mx, float my) {
	game->getGAM()->playSound(C_SWING);
	setCurrentState(direction==1?ATTACKING_RIGHT:ATTACKING_LEFT);

	float px = getCurrentBodyX() * BOX2D_CONVERSION_FACTOR;
	float py = getCurrentBodyY() * BOX2D_CONVERSION_FACTOR;
	py = game->getGSM()->getWorld()->getWorldHeight() - py;

	AnimatedSpriteType *bubbleSpriteType = game->getGSM()->getSpriteManager()->getSpriteType(9);
	float bubbleX;
	if(mx > px){
		// Right side click
		setDirection(1);
		setCurrentState(ATTACKING_RIGHT);
		bubbleX = px + game->getGSM()->getSpriteManager()->getPlayer()->getSpriteType()->getTextureWidth() / 2.0;
	}
	else {
		// Left side click
		setDirection(-1);
		setCurrentState(ATTACKING_LEFT);
		bubbleX = px - bubbleSpriteType->getTextureWidth()*2;
	}

	Vine *bubble = new Vine(PROJECTILE_DESIGNATION);
	bubble->init(bubbleX,py,bubbleSpriteType);

	//create a physics object for the seed
	game->getGSM()->getBoxPhysics()->getPhysicsFactory()->createPlayerObject(game,bubble,false);
	game->getGSM()->getPhysics()->addCollidableObject(bubble);
	game->getGSM()->getSpriteManager()->addBot(bubble);

	// Remove this bubble after 15 frames
	game->getGSM()->getSpriteManager()->addBotToRemovalList(bubble, 15);
}

void  FOR_Player::waterAttackR(Game* game, float mx, float my) {
	game->getGAM()->playSound(C_SWING);
	setCurrentState(direction==1?ATTACKING_RIGHT:ATTACKING_LEFT);

	float px = game->getGSM()->physicsToScreenX(getCurrentBodyX());
	float py = game->getGSM()->physicsToScreenY(getCurrentBodyY());
	//py = game->getGSM()->getWorld()->getWorldHeight() - py;

	AnimatedSpriteType *fountainSpriteType = game->getGSM()->getSpriteManager()->getSpriteType(10);
	float fountainX;
	if(mx > px){
		// Right side click
		setDirection(1);
		setCurrentState(ATTACKING_RIGHT);
		fountainX = px + game->getGSM()->getSpriteManager()->getPlayer()->getSpriteType()->getTextureWidth() / 2.0;
	}
	else {
		// Left side click
		setDirection(-1);
		setCurrentState(ATTACKING_LEFT);
		fountainX = px - fountainSpriteType->getTextureWidth()*2;
	}

	Vine *fountain = new Vine(PROJECTILE_DESIGNATION);
	fountain->init(fountainX,py,fountainSpriteType);

	//create a physics object for the fountain
	game->getGSM()->getBoxPhysics()->getPhysicsFactory()->createPlayerObject(game,fountain,false);
	game->getGSM()->getPhysics()->addCollidableObject(fountain);
	game->getGSM()->getSpriteManager()->addBot(fountain);

	// Remove this vine after 15 frames
	game->getGSM()->getSpriteManager()->addBotToRemovalList(fountain, 15);
}

void FOR_Player::fireAttackL(Game *game, float mx, float my) {
	game->getGAM()->playSound(C_SWING);
	setCurrentState(direction==1?ATTACKING_RIGHT:ATTACKING_LEFT);

	float px = game->getGSM()->physicsToScreenX(getCurrentBodyX());
	float py = game->getGSM()->physicsToScreenY(getCurrentBodyY());
	//py = game->getGSM()->getWorld()->getWorldHeight() - py;

	AnimatedSpriteType *flamethrowerSpriteType = game->getGSM()->getSpriteManager()->getSpriteType(8);
	float flamethrowerX;
	if(mx > px){
		// Right side click
		setDirection(1);
		setCurrentState(ATTACKING_RIGHT);
		flamethrowerX = px + game->getGSM()->getSpriteManager()->getPlayer()->getSpriteType()->getTextureWidth() / 2.0;
	}
	else {
		// Left side click
		setDirection(-1);
		setCurrentState(ATTACKING_LEFT);
		flamethrowerX = px - flamethrowerSpriteType->getTextureWidth()*2;
	}

	Vine *flamethrower = new Vine(PROJECTILE_DESIGNATION);
	flamethrower->init(flamethrowerX,py,flamethrowerSpriteType);

	//create a physics object for the flamethrower
	game->getGSM()->getBoxPhysics()->getPhysicsFactory()->createPlayerObject(game,flamethrower,false);
	game->getGSM()->getPhysics()->addCollidableObject(flamethrower);
	game->getGSM()->getSpriteManager()->addBot(flamethrower);

	// Remove this vine after 15 frames
	game->getGSM()->getSpriteManager()->addBotToRemovalList(flamethrower, 15);
}

void  FOR_Player::fireAttackR(Game* game, float mx, float my) {
	game->getGAM()->playSound(C_SWING);
	// Get mouse and player locations
	float px = game->getGSM()->physicsToScreenX(getCurrentBodyX());
	float py = game->getGSM()->physicsToScreenY(getCurrentBodyY());

	// Get the difference bewteen these loactions
	float difX = mx - px;
	float difY = my - py;

	if(difX > 0){
		setDirection(1);
		setCurrentState(ATTACKING_RIGHT);
	} else {
		setDirection(-1);
		setCurrentState(ATTACKING_LEFT);
	}

	// Get the length of the vector from player to mouse
	float length = std::sqrt( (difX * difX) + (difY * difY) );

	// Normalize the distances
	difX /= length;
	difY /= length;

	// Scale distances to be x and y velocity
	difX *= PROJECTILE_VELOCITY;
	difY *= PROJECTILE_VELOCITY;

	// Fire Ball
	AnimatedSpriteType *fireballSpriteType = game->getGSM()->getSpriteManager()->getSpriteType(7);
	FireBall *fireball = new FireBall(PROJECTILE_DESIGNATION, difX, -difY);
	fireball->init(px,py,fireballSpriteType);

	//create a physics object for the seed
	game->getGSM()->getBoxPhysics()->getPhysicsFactory()->createFriendlyProjectile(game,fireball,true);
	game->getGSM()->getPhysics()->addCollidableObject(fireball);
	game->getGSM()->getSpriteManager()->addBot(fireball);

	// Set the velocity of the fireball
	fireball->getPhysicsBody()->SetLinearVelocity(b2Vec2(difX, -difY));
}