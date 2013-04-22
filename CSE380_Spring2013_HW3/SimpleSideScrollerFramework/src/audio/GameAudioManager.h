#pragma once

#include "stdafx.h"
#include "src\game\Game.h"
#include "src\game\WStringTable.h"
#include "irrKlang.h"

class GameAudioManager{

protected:
	irrklang::ISoundEngine* engine;
	irrklang::ISound* currentMusic;
	list<irrklang::ISound*> currentSounds;

	map<string, irrklang::ISoundSource*> soundBank;

	float musicVolume;
	float soundVolume;
	float gameVolume;
public:

	void addSound(char *path);
	void addMusic(char *path);
	void playSound(char *alias);
	void initMusic(char *alias);
	void setMusic(char *alias);

	void update();
	//in-line maintenence methods

	void reloadAudio(){ engine->removeAllSoundSources(); }
	void shutdown();
	GameAudioManager();
	~GameAudioManager();
};