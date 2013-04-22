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
	map<string, irrklang::ISoundSource*> musicBank;

	float musicVolume;
	float soundVolume;
	float gameVolume;
public:

	void addSound(char *path, char *alias);
	void addMusic(char *path, char *alias);
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