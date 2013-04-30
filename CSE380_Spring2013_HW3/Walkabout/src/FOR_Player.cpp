
#include "stdafx.h"
#include "src\Seed.h"
#include "src\game\Game.h"
#include "src\gsm\sprite\SpriteManager.h"
#include "src\gui\ScreenGUI.h"
#include "src\WalkaboutGame.h"
#include "FOR_Player.h"
#include "src\gsm\sprite\SpriteDesignations.h"
#include "src\audio\GameAudioManager.h"


// Initialize stuff, att_Cooldown -> The number of frames between player attacks
FOR_Player::FOR_Player(int att_Cooldown, int d_Cooldown, int designation){
	dead = false;
	stunned = false;
	cd_Counter = 0;
	death_Cooldown = d_Cooldown;
	attack_Cooldown = att_Cooldown;
	selected_element = NOTHING;
	this->designation = designation;
}

// Can this player attack?  Assumes that the player is going to attack if they can
bool FOR_Player::can_Attack(){
	if(stunned)
		return false;
	if(cd_Counter <= 0){
		cd_Counter = attack_Cooldown;
		return true;
	}
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
	int hp = this->getHitPoints()/(MAX_HP/10);
	if (this->getHitPoints()<0)
		hp = 0;
	gui->popOverlayImage();
	gui->popOverlayImage();
	gui->popOverlayImage();

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
}

void FOR_Player::playSound(Game* game, SpriteDesignations soundType){

	if(soundType == SPRITE_DEAD && !dead){
		game->getGAM()->playSound(C_DEATH);
	}

	if(soundType == SPRITE_HIT){
		game->getGAM()->playSound(C_PLAYERHIT);
	}
}

void FOR_Player::stun() {
	// If the player is dead, don't do anything if gets stunned
	if(!dead && !stunned) {
		stunned = true;
		cd_Counter = attack_Cooldown;
	}
}