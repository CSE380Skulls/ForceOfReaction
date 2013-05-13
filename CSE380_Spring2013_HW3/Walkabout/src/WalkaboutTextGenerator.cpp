/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	WalkaboutTextGenerator.cpp

	See WalkaboutTextGenerator.h for a class description.
*/

#include "stdafx.h"
#include "src\WalkaboutGame.h"
#include "src\WalkaboutTextGenerator.h"
#include "src\game\Game.h"
#include "src\graphics\GameGraphics.h"
#include "src\text\GameText.h"
#include "src\timer\GameTimer.h"
#include "src\PlatformPlugins\WindowsPlugin\WindowsTimer.h"

static const wstring	W_TARGET_FPS_TEXT = L"TARGET FPS: ";
static const int		W_TARGET_FPS_X	= 350;
static const int		W_TARGET_FPS_Y	= 5;

/*
	appendTargetFPS - this method gets and appends the target frame rate to
	the text we are generating.
*/
void WalkaboutTextGenerator::appendTargetFPS(Game *game)
{

	// WE'LL BUILD OUR WIDE STRING USING A WIDE STRING STREAM
	wstringstream wss;
	WindowsTimer *timer = (WindowsTimer*)game->getTimer();

	// BUILD THE WIDE STRING
	//textToGenerate.append(W_TARGET_FPS_TEXT);
	wss << timer->getTargetFPS();
	//textToGenerate.append(wss.str());
}

/*
	initText - Provides an example of how to render text to our screen every frame,
	allowing for the text to continually change. This method adds a text object
	to the text manager (GameText), which allows it to be updated each frame. This
	method should be called only once for each object.
*/
void WalkaboutTextGenerator::initText(Game *game)
{
	
	// FIRST UPDATE THE TEXT THIS GENERATOR IS USING
	appendTargetFPS(game);

	// AND THEN HAND IT TO THE TEXT MANAGER, SPECIFYING WHERE IT SHOULD BE RENDERED
	GameText *text = game->getText();
	text->addText(&textToGenerate, W_TARGET_FPS_X, W_TARGET_FPS_Y, game->getGraphics()->getScreenWidth(), game->getGraphics()->getScreenHeight());
	
}

/*
	updateText - updates the text. Note that the text must already have been added
	to the text manager via the initText method. Also not that there should be a
	separate TextGenerator for each line of text we wish to render.
*/
void WalkaboutTextGenerator::updateText(Game *game)
{
	// THE TEXT MANAGER (GameText) HAS A POINTER TO OUR TEXT, SO WE CAN
	// SIMPLY CLEAR IT OUT OF OLD CHARACTER DATA AND FILL IT IN WITH
	// WHAT WE WANT.
	
	textToGenerate.clear();
	appendTargetFPS(game);

	if(game->getGSM()->isGameInProgress()){
		wstringstream wss;
		wss << L"Player HealtH: ";
		wss << game->getGSM()->getSpriteManager()->getPlayer()->getHitPoints();
		//textToGenerate.append(wss.str());
	}
}