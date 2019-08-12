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


mapflip::mapflip(const FilePath& path, int yFlipHeight, int xFlipWidth) {
	map = new Texture(path);
	this->yFlipHeight = yFlipHeight;
	this->xFlipWidth = xFlipWidth;
	yMapHeight = map->height();
	xMapWidth = map->width();
	nowPosX = 0;
	nowPosY = 0;
}
mapflip::~mapflip() {
	delete map;
}
bool mapflip::nextFlip() {
	if (nowPosX += xFlipWidth, nowPosX >= xMapWidth) {	//次の列へ
		nowPosX = 0;
		if (nowPosY += yFlipHeight, nowPosY >= yMapHeight) {	//次の行の一番左の列へ
			nowPosY = 0;
			return false;
		}
	}
	return true;
}
TextureRegion* mapflip::flipFromMap() {
	return &(*map)(nowPosX, nowPosY, xFlipWidth, yFlipHeight);
}


flipMovie::flipMovie(const FilePath& path, int yFlipHeight, int xFlipWidth, int drawX, int drawY) {
	flip = new mapflip(path, yFlipHeight, xFlipWidth);
	this->drawX = drawX;
	this->drawY = drawY;

}
flipMovie::~flipMovie() {
	delete flip;
}
bool flipMovie::update(double t) {
	if (flip->nextFlip() == false) {
		return false;
	}
	(*flip)()->drawAt(drawX, drawY);
}

