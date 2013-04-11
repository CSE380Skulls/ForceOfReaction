/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	WalkaboutTextGenerator.h

	This class is a custom text generator for the EmptyGame
	application. Basically, by defining the initText and updateText
	methods, we can have it display whatever text we like.
*/

#pragma once
#include "stdafx.h"
#include "src\text\TextGenerator.h"

class WalkaboutTextGenerator : public TextGenerator
{
private:
	// THIS IS THE TEXT THIS OBJECT WILL UPDATE EACH FRAME
	wstring textToGenerate;

public:
	// NOTHING TO INITIALIZE OR DESTROY
	WalkaboutTextGenerator()	{}
	~WalkaboutTextGenerator()	{}

	// DEFINED IN WalkaboutTextGenerator.cpp
	void appendTargetFPS(Game *game);
	void initText(Game *game);
	void updateText(Game *game);
};