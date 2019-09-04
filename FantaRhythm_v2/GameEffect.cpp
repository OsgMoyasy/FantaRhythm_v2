#include "GameEffect.h"

void GameEffect::playSE(SE::NAME name, SE::TYPE type) {
	static Audio* hit = new Audio(U"resources/musics/effects/decision.mp3");//�b�艹���t�@�C��

	Audio* sound = NULL;
	switch (name) {
	case SE::HIT:
		sound = hit;
		break;
	}

	switch (type) {
	case SE::DUL:
		sound->playOneShot();
		break;
	case SE::REP:
		sound->stop();
		sound->stopAllShots();
		sound->play();
		break;
	case SE::IGN:
		sound->play();
		break;
	}
}

void GameEffect::playVE(VE::NAME name) {
	//������(Siv3D��Effect�N���X���g������)
}

void GameEffect::playME(ME::NAME name) {
	switch (name) {
	case ME::HIT:
		playSE(SE::HIT, SE::REP);
		playVE(VE::HIT);
	}
}