#include "GameEffect.h"

/*
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
*/

SE::SE(FilePath& path) {
	sound = new Audio(path);
}
SE::~SE() {
	delete sound;
}
void SE::play() {
	sound->stop();
	sound->play();
}


mapchip::mapchip(FilePath& path, int yChipHeight, int xChipWidth) {
	map = new Texture(path);
	this->yChipHeight = yChipHeight;
	this->xChipWidth = xChipWidth;
	yMapHeight = map->height();
	xMapWidth = map->width();
}
mapchip::~mapchip() {
	delete map;
}
TextureRegion mapchip::chipFromMap() {
	static int nowPosX = 0;
	static int nowPosY = 0;
	TextureRegion chip = (*map)(nowPosX, nowPosY, xChipWidth, yChipHeight);

	if (nowPosX += xChipWidth, nowPosX >= xMapWidth) {
		nowPosX = 0;
		if (nowPosY += yChipHeight, nowPosY >= yMapHeight) {
			nowPosY = 0;
		}
	}

	return chip;
}
