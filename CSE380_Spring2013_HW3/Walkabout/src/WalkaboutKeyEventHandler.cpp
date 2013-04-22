/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	WalkaboutKeyEventHandler.cpp

	See WalkaboutKeyEventHandler.h for a class description.
*/

#include "stdafx.h"
#include "src\WalkaboutGame.h"
#include "src\WalkaboutKeyEventHandler.h"
#include "src\Seed.h"
#include "src\game\Game.h"
#include "src\game\WStringTable.h"
#include "src\graphics\GameGraphics.h"
#include "src\gsm\physics\Physics.h"
#include "src\gsm\physics\PhysicalProperties.h"
#include "src\gsm\sprite\AnimatedSprite.h"
#include "src\gsm\state\GameState.h"
#include "src\gsm\state\GameStateManager.h"
#include "src\gui\Cursor.h"
#include "src\gui\GameGUI.h"
#include "src\input\GameInput.h"
#include "src\timer\GameTimer.h"
#include "src\PlatformPlugins\WindowsPlugin\WindowsTimer.h"

/*
	handleKeyEvent - this method handles all keyboard interactions. Note that every frame this method
	gets called and it can respond to key interactions in any custom way. Ask the GameInput class for
	key states since the last frame, which can allow us to respond to key presses, including when keys
	are held down for multiple frames.
*/
void WalkaboutKeyEventHandler::handleKeyEvents(Game *game)
{
	// WE CAN QUERY INPUT TO SEE WHAT WAS PRESSED
	GameInput *input = game->getInput();

	// LET'S GET THE PLAYER'S PHYSICAL PROPERTIES, IN CASE WE WANT TO CHANGE THEM
	GameStateManager *gsm = game->getGSM();
	AnimatedSprite *player = gsm->getSpriteManager()->getPlayer();
	PhysicalProperties *pp = player->getPhysicalProperties();	

	// IF THE GAME IS IN PROGRESS
	if (gsm->isGameInProgress())
	{
		// WASD KEY PRESSES WILL CONTROL THE PLAYER
		// SO WE'LL UPDATE THE PLAYER VELOCITY WHEN THESE KEYS ARE
		// PRESSED, THAT WAY PHYSICS CAN CORRECT AS NEEDED

		if (input->isKeyDown(A_KEY))
		{

			player->getPhysicsBody()->SetLinearVelocity(b2Vec2(-PLAYER_VELOCITY,
				player->getPhysicsBody()->GetLinearVelocity().y));

			if (player->getPhysicsBody()->GetLinearVelocity().y==0)
				player->setCurrentState(WALKING_LEFT);

		}
		else if (input->isKeyDown(D_KEY))
		{

			player->getPhysicsBody()->SetLinearVelocity(b2Vec2(PLAYER_VELOCITY,
				player->getPhysicsBody()->GetLinearVelocity().y));

			if (player->getPhysicsBody()->GetLinearVelocity().y==0)
				player->setCurrentState(WALKING_RIGHT);
		}
		else
		{
			player->getPhysicsBody()->SetLinearVelocity(b2Vec2(0, player->getPhysicsBody()->GetLinearVelocity().y));
			if(player->getCurrentState() == WALKING_RIGHT){
				if(player->getPhysicsBody()->GetLinearVelocity().y < 0.0f)
					player->setCurrentState(FALLING_RIGHT);
				else
					player->setCurrentState(IDLE_RIGHT);
			}
			if(player->getCurrentState() == WALKING_LEFT){
				if(player->getPhysicsBody()->GetLinearVelocity().y < 0.0f)
					player->setCurrentState(FALLING_LEFT);
				else
					player->setCurrentState(IDLE_LEFT);
			}
			if((player->getCurrentState() == JUMPING_RIGHT || player->getCurrentState() == FALLING_RIGHT) 
				&& player->getPhysicsBody()->GetLinearVelocity().y == 0.0f)
				player->setCurrentState(IDLE_RIGHT);
			if((player->getCurrentState() == JUMPING_LEFT || player->getCurrentState() == FALLING_LEFT)
				&& player->getPhysicsBody()->GetLinearVelocity().y == 0.0f)
				player->setCurrentState(IDLE_LEFT);
			if(player->getCurrentState() == ATTACKING_RIGHT && player->getFrameIndex()==12)
				player->setCurrentState(IDLE_RIGHT);
			if(player->getCurrentState() == ATTACKING_LEFT && player->getFrameIndex()==12)
				player->setCurrentState(IDLE_LEFT);
		}
		if (input->isKeyDownForFirstTime(SPACE_KEY))
		{
			//Need to create a box2d listener to set a similiar "wasOnTileLastFrame"
			//condition, otherwise he will continually jump even in midair :)
			//TODO: get a better understanding of mass in this world
			//		and how the velocity affects it, this high vel is
			//		not right...
			if(player->getPhysicsBody()->GetLinearVelocity().y==0)
			{
				player->getPhysicsBody()->ApplyLinearImpulse(b2Vec2(0.0f, JUMP_VELOCITY),
					player->getPhysicsBody()->GetPosition());
			
				if(player->getCurrentState() == WALKING_RIGHT || player->getCurrentState()==IDLE_RIGHT)
					player->setCurrentState(JUMPING_RIGHT);

				if(player->getCurrentState() == WALKING_LEFT || player->getCurrentState()==IDLE_LEFT)
					player->setCurrentState(JUMPING_LEFT);
			}
		}
		if (input->isKeyDownForFirstTime(ESCAPE_KEY))
		{
			if (gsm->getPhysics()->isActivated())
				gsm->getPhysics()->togglePhysics();
			gsm->goToPaused();
		}
		if (input->isKeyDownForFirstTime(T_KEY))
		{
			gsm->getPhysics()->activateForSingleUpdate();
		}
		if (input->isKeyDownForFirstTime(MOUSE_LEFT))
		{
			if(player->getCurrentState() == WALKING_RIGHT || player->getCurrentState()==IDLE_RIGHT)
				player->setCurrentState(ATTACKING_RIGHT);
			if(player->getCurrentState() == WALKING_LEFT || player->getCurrentState()==IDLE_LEFT)
				player->setCurrentState(ATTACKING_LEFT);
		}
		if(input->isKeyDownForFirstTime(MOUSE_RIGHT))
		{
			// Get mouse and player locations
			float mX = game->getGUI()->getCursor()->getX() + game->getGUI()->getViewport()->getViewportX();
			float mY = game->getGUI()->getCursor()->getY() + game->getGUI()->getViewport()->getViewportY();
			float pX = game->getGSM()->getSpriteManager()->getPlayer()->getCurrentBodyX() * BOX2D_CONVERSION_FACTOR;
			float pY = game->getGSM()->getSpriteManager()->getPlayer()->getCurrentBodyY() * BOX2D_CONVERSION_FACTOR;
			pY = game->getGSM()->getWorld()->getWorldHeight() - pY;

			// Get the difference bewteen these loactions
			float difX = mX - pX;
			float difY = mY - pY;

			if(difX > 0){
				player->setCurrentState(ATTACKING_RIGHT);
			}
			else {
				player->setCurrentState(ATTACKING_LEFT);
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
			Seed *seed = new Seed();
			seed->setSpriteType(seedSpriteType);
			seed->setAlpha(255);
			seed->setCurrentState(IDLE_LEFT);
			PhysicalProperties *seedProps = seed->getPhysicalProperties();
			seedProps->setX(pX);
			seedProps->setY(pY);
			seedProps->setVelocity(0.0f, 0.0f);
			seedProps->setAccelerationX(0);
			seedProps->setAccelerationY(0);
			seed->setOnTileThisFrame(false);
			seed->setOnTileLastFrame(false);
			seed->affixTightAABBBoundingVolume();

			//create a physics object for the seed
			game->getGSM()->getBoxPhysics()->getPhysicsFactory()->createPlayerObject(game,seed);

			// Set the velocity of the seed
			seed->getPhysicsBody()->SetLinearVelocity(b2Vec2(difX, -difY));
			seed->setHitPoints(10);

			game->getGSM()->getPhysics()->addCollidableObject(seed);
			game->getGSM()->getSpriteManager()->addBot(seed);
		
		}
		// CTRL + 1 = auto win level
		if(input->isKeyDown(CTRL_KEY) && input->isKeyDownForFirstTime(ONE_KEY)){
			if (gsm->getPhysics()->isActivated())
				gsm->getPhysics()->togglePhysics();
			gsm->goToLevelWon();
		}
		// CTRL + 2 = auto lose level (temporary until actuall impemented)
		if(input->isKeyDown(CTRL_KEY) && input->isKeyDownForFirstTime(TWO_KEY)){
			if (gsm->getPhysics()->isActivated())
				gsm->getPhysics()->togglePhysics();
			gsm->goToGameOver();
		}

		bool viewportMoved = false;
		float viewportVx = 0.0f;
		float viewportVy = 0.0f;
		Viewport *viewport = game->getGUI()->getViewport();

		 float x = game->getGSM()->getSpriteManager()->getPlayer()->getCurrentBodyX();
		 float y = game->getGSM()->getSpriteManager()->getPlayer()->getCurrentBodyY();
		 float pX = x * game->getGSM()->getWorld()->getWorldConvFactor();
		 float pY = y * game->getGSM()->getWorld()->getWorldConvFactor();
		 pY = game->getGSM()->getWorld()->getWorldHeight() - pY;

		 float vpX = viewport->getViewportX();
		 float vpY = viewport->getViewportY();
		 float vpWidth = viewport->getViewportWidth();
		 float vpHeight = viewport->getViewportHeight();
		 
		 // If player is on left 3/8 of viewport
		 if(pX < ((3.0f / 8.0f * vpWidth) + vpX)) {
			viewportVx = -MAX_VIEWPORT_AXIS_VELOCITY; 
			viewportMoved = true;
		 }

		 // If player is on right 5/8 of viewport
		 if(pX > ((5.0f / 8.0f * vpWidth) + vpX)) {
			viewportVx = MAX_VIEWPORT_AXIS_VELOCITY; 
			viewportMoved = true;
		 }

		 // If player is on top 3/8 of viewport
		 if(pY < ((3.0f / 8.0f * vpHeight) + vpY)) {
			viewportVy = -MAX_VIEWPORT_AXIS_VELOCITY; 
			viewportMoved = true;
		 }

		 // If player is on bottom 5/8 of viewport
		 if(pY > ((5.0f / 8.0f * vpHeight) + vpY)) {
			viewportVy = MAX_VIEWPORT_AXIS_VELOCITY; 
			viewportMoved = true;
		 }
		
		if (viewportMoved)
			viewport->moveViewport((int)viewportVx, (int)viewportVy, game->getGSM()->getWorld()->getWorldWidth(), game->getGSM()->getWorld()->getWorldHeight());
		
	}

	// LET'S MESS WITH THE TARGET FRAME RATE IF THE USER PRESSES THE HOME OR END KEYS
	WindowsTimer *timer = (WindowsTimer*)game->getTimer();
	int fps = timer->getTargetFPS();

	// THIS SPEEDS UP OUR GAME LOOP AND THUS THE GAME, NOTE THAT WE COULD ALTERNATIVELY SCALE
	// DOWN THE GAME LOGIC (LIKE ALL VELOCITIES) AS WE SPEED UP THE GAME. THAT COULD PROVIDE
	// A BETTER PLAYER EXPERIENCE
	if (input->isKeyDown(VK_HOME) && (fps < MAX_FPS))
		timer->setTargetFPS(fps + FPS_INC);

	// THIS SLOWS DOWN OUR GAME LOOP, BUT WILL NOT GO BELOW 5 FRAMES PER SECOND
	else if (input->isKeyDown(VK_END) && (fps > MIN_FPS))
		timer->setTargetFPS(fps - FPS_INC);
}