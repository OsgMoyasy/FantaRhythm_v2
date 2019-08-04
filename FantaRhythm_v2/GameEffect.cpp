#include "GameEffect.h"

void GameEffect::playSE(SE::NAME name, SE::TYPE type) {
	static Audio* hit = new Audio(U"resources/musics/effects/decision.mp3");//暫定音声ファイル

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
	//未実装(Siv3DのEffectクラスを使うつもり)
}

void GameEffect::playME(ME::NAME name) {
	switch (name) {
	case ME::HIT:
		playSE(SE::HIT, SE::REP);
		playVE(VE::HIT);
	}
}