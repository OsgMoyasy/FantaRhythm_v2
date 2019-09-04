#include "MusicManager.h"

Audio* MusicManager::audio;

void MusicManager::playMusicPrev(String path) {
	delete audio;
	audio = new Audio(path + U"/preview.wav");
	audio->setLoop(true);
	audio->play();
}

void MusicManager::playMusicGame(String path) {
	delete audio;
	audio = new Audio(path+ U"/game.wav");
	audio->play();
}

double MusicManager::getMusicTime() {
	return ((double)audio->posSample() / (double)audio->samplingRate()) * 1000;
}

double MusicManager::getMilliSec() {
	return audio->samplingRate() / 1000.0;
}