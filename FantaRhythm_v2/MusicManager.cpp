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
	if (stopwatch.isRunning()) {//�v�����̏ꍇ
		if (stopwatch.s() >= 6) {//�I�������T�b��ɏI��
			return true;
		}
		return false;
	}
	if (audio->lengthSec() - 1 <= audio->posSec()) {//�Đ����I�����Ă���
		stopwatch.start();//�^�C�}�[start
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