/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	GameStateManager.h

	This class manages the static data for the game level being
	used. This means all game backgrounds, which are stored and
	manipulated in WorldLayer objects. A given level can have
	a game background rendered using multiple layers, some which
	can be tiled, others that can be sparse, and others that
	can be isometric.

	This class stores these layers and ensures they are rendered
	in the proper order, low index to high. Layers that need to
	be drawn first (the back-most layer), should be added first.
*/
#pragma once
#include "stdafx.h"
#include "src\gui\OverlayImage.h"

class Game;

static const int NO_IMAGES = 0;

class Cutscene
{
private:
	vector<OverlayImage*> overlayImages;
	int numImages;
	int currentImage;

public:

	// METHODS DEFINED in Cutscene.cpp
	Cutscene();
	~Cutscene();
	void			addImage(OverlayImage *image);
	OverlayImage*	nextImage();

	OverlayImage*	initImage() { return overlayImages.at(0); }
	bool			isFinished() { return currentImage==numImages; }
};