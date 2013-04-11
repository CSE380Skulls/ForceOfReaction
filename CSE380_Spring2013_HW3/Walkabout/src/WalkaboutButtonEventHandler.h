/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	WalkaboutButtonEventHandler.h

	This is a custom button event handler. Each game can choose
	to use different buttons and respond differently to when
	buttons are pressed. This class will handle button presses for
	the WalkaboutGame following the ButtonEventHandler pattern.
*/

#pragma once
#include "stdafx.h"
#include "src\input\ButtonEventHandler.h"

class Game;

class WalkaboutButtonEventHandler: public ButtonEventHandler
{
public:
	// AS-IS, THIS CLASS HAS NO DATA, SO THERE IS NOTHING
	// TO INITIALIZE OR DESTROY
	WalkaboutButtonEventHandler()	{}
	~WalkaboutButtonEventHandler()	{}

	// DEFINED INSIDE WalkaboutButtonEventHandler.cpp
	void handleButtonEvents(Game *game, 
							wstring command);
};