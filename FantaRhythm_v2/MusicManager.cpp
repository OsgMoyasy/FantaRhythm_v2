#include "MusicManager.h"

Audio* MusicManager::audio;
Stopwatch MusicManager::stopwatch;

void MusicManager::playMusicPrev(String path) {
	delete audio;
	audio = new Audio(path + U"/preview.wav");
	audio->setLoop(true);
	audio->play();
}

void MusicManager::setMusicGame(String path) {
	delete audio;
	stopwatch.reset();
	audio = new Audio(path+ U"/game.wav");
}

double MusicManager::getMusicTime() {
	return ((double)audio->posSample() / (double)audio->samplingRate()) * 1000;
}

double MusicManager::getMilliSec() {
	return audio->samplingRate() / 1000.0;
}

bool MusicManager::musicEndCheck() {
	if (stopwatch.isRunning()) {//計測中の場合
		if (stopwatch.s() >= 6) {//終了から約５秒後に終了
			return true;
		}
		return false;
	}
	if (audio->lengthSec() - 1 <= audio->posSec()) {//再生が終了している
		stopwatch.start();//タイマーstart
	}
	return false;
}

void MusicManager::pauseMusic() {
	audio->pause();
}

void MusicManager::playMusic() {
	audio->play();
}

void MusicManager::setEndMusic() {
	audio->setPosSec(audio->lengthSec());
}