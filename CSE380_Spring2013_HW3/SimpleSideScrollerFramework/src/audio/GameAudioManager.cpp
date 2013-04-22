#include "stdafx.h"
#include "src\game\Game.h"
#include "src\audio\GameAudioManager.h"
using namespace irrklang;

GameAudioManager::GameAudioManager()
{ 
	engine = createIrrKlangDevice(); 
	musicVolume=1.0f;
	soundVolume=1.0f;
	gameVolume=1.0f;
}
GameAudioManager::~GameAudioManager()
{ 
}
void GameAudioManager::addSound(char *path, char *alias)
{
	ISoundSource *iss = engine->addSoundSourceFromFile(path);
	soundBank[alias]=iss;
}
void GameAudioManager::addMusic(char *path, char *alias)
{
	ISoundSource *iss = engine->addSoundSourceFromFile(path);
	musicBank[alias]=iss;
}
void GameAudioManager::playSound(char *alias)
{
	ISound *is = engine->play2D(soundBank[alias],false,false,true);
	is->setVolume(soundVolume);
	is->setIsLooped(false);
	currentSounds.push_front(is);
}

void GameAudioManager::initMusic(char *alias)
{
	ISoundSource* is = musicBank[alias];
	currentMusic = engine->play2D(musicBank[alias],false,false,true);
	currentMusic->setVolume(musicVolume);
	currentMusic->setIsLooped(true);
}
void GameAudioManager::setMusic(char *alias)
{
	currentMusic->stop();
	currentMusic = engine->play2D(musicBank[alias],false,false,true);
	currentMusic->setVolume(musicVolume);
	currentMusic->setIsLooped(true);
}

void GameAudioManager::update()
{
	list<irrklang::ISound*>::iterator soundIterator;
	soundIterator = currentSounds.begin();
	while (soundIterator != currentSounds.end())
	{			
		ISound* sound = (*soundIterator);
		if (sound->isFinished())
		{
			currentSounds.remove(sound);
			sound->drop();
		}
		else
			soundIterator++;
	}
}

void GameAudioManager::shutdown()
{
	currentMusic->stop();
	currentMusic->drop();
	engine->removeAllSoundSources();
	engine->drop(); 
}