/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	WalkaboutDataLoader.h

	This class provides a custom importer for the WalkaboutGame to import
	game data, gui data, and world (i.e. level) data.
*/
#pragma once
#include "stdafx.h"
#include "src\dataLoader\GameDataLoader.h"
#include "src\game\Game.h"
#include "src\gsm\sprite\AnimatedSpriteType.h"
#include "src\PlatformPlugins\DirectXPlugin\DirectXTextureManager.h"

class WalkaboutDataLoader : public GameDataLoader
{
private:
	// WE NEED THESE GUYS TO INIT OUR WINDOWS WINDOW
	HINSTANCE hInstance;
	int nCmdShow;

public:
	WalkaboutDataLoader()	{}
	~WalkaboutDataLoader()	{}

	// INLINED MUTATOR METHOD
	void initWinHandle(HINSTANCE initHInstance, int initNCmdShow)
	{
		hInstance = initHInstance;
		nCmdShow = initNCmdShow;
	}


	// THESE SHOULD BE LOADING DATA FROM FILES. THEY
	// ARE ALL DEFINED IN WalkaboutDataLoader.cpp
	void loadGame(Game *game, wstring gameInitFile);
	void loadGUI(Game *game, wstring guiInitFile);
	void loadWorld(Game *game, wstring levelInitFile);
	void loadAudio(Game* game, wstring audioInitFile);

	// THESE ARE HARD-CODED EXAMPLES OF GUI DATA LOADING
	void hardCodedLoadGUIExample(Game *game);
	void hardCodedLoadAudioExample(Game* game);

	// AND SOME HELPER METHODS FOR LOADING THE GUI
	void initCursor(GameGUI *gui, DirectXTextureManager *guiTextureManager);
	void initSplashScreen(Game *game, GameGUI *gui,	DirectXTextureManager *guiTextureManager);
	void initPaused(GameGUI *gui, DirectXTextureManager *guiTextureManager);
	void initMainMenu(GameGUI *gui,	DirectXTextureManager *guiTextureManager);
	void initInGameGUI(GameGUI *gui, DirectXTextureManager *guiTextureManager);
	void initCredits(GameGUI *gui, DirectXTextureManager *guiTextureManager);
	void initOptions(GameGUI *gui, DirectXTextureManager *guiTextureManager);
	void initLoading(GameGUI *gui, DirectXTextureManager *guiTextureManager);
	void initLevelWon(GameGUI *gui, DirectXTextureManager *guiTextureManager);
	void initGameOver(GameGUI *gui, DirectXTextureManager *guiTextureManager);
	void initCutscene(Game *game, GameGUI *gui,	DirectXTextureManager *guiTextureManager);
};