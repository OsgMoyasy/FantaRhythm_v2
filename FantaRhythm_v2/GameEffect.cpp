#include "GameEffect.h"

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


mapflip::mapflip(const FilePath& path, int xFlipWidth, int yFlipHeight) {
	flip = new Texture(path);
	this->xFlipWidth = xFlipWidth;
	this->yFlipHeight = yFlipHeight;
	xMapWidth = flip->width();
	yMapHeight = flip->height();
	nowPosX = 0;
	nowPosY = 0;
}
mapflip::~mapflip() {
	delete flip;
}
bool mapflip::nextFlip() {
	if (nowPosX += xFlipWidth, nowPosX >= xMapWidth) {	//次の列へ
		nowPosX = 0;
		if (nowPosY += yFlipHeight, nowPosY >= yMapHeight) {	//次の行の一番左の列へ
			return false;
		}
	}
	return true;
}
TextureRegion mapflip::getFlip() {
	return (*flip)(nowPosX, nowPosY, xFlipWidth, yFlipHeight);
}


flipMovie::flipMovie(const FilePath& path, int xFlipWidth, int yFlipHeight, int drawX, int drawY) {
	flip = new mapflip(path, xFlipWidth, yFlipHeight);
	this->drawX = drawX;
	this->drawY = drawY;
}
flipMovie::~flipMovie() {
	delete flip;
}
bool flipMovie::update(double t) {
	if (!flip->nextFlip()) {
		return false;
	}
	flip->getFlip().drawAt(drawX, drawY);
	return true;
}


fractal::fractal(int size, int x, int y) {
	constexpr int angle = 30;
	constexpr double PI = 3.1415;
	firstX = x;
	firstY = y;
	firstSize = size;
	x2 = size * cos(angle * (PI / 180));
	y2 = size * cos((90 - angle) * (PI / 180));
}
bool fractal::update(double t) {
	if (t > 5) {return false;}
	drawTriangle(t, firstX, firstY, firstSize, x2, y2);
	return true;
}
void fractal::drawTriangle(double t, int posX, int posY, int size, int x2, int y2) {
	//コードが汚い(趣味で書いただけなので後回し)
	if (t > 1) {
		drawTriangle(t - 1, posX, posY - size / 2, size / 2, x2 / 2, y2 / 2);
		drawTriangle(t - 1, posX - x2 / 2, posY + y2 / 2, size / 2, x2 / 2, y2 / 2);
		drawTriangle(t - 1, posX + x2 / 2, posY + y2 / 2, size / 2, x2 / 2, y2 / 2);
		t = 1.0;
	}
	int aX = posX;
	int aY = posY - size;
	int bX = posX - x2;
	int bY = posY + y2;
	int cX = posX + x2;
	int cY = posY + y2;
	int abX = (-x2) * t;
	int abY = (size + y2) * t;
	int bcX = (x2 * 2) * t;
	int bcY = 0;
	int caX = (-x2) * t;
	int caY = (-y2 - size) * t;
	Line(aX, aY, aX + abX, aY + abY).draw(2, Palette::Red);
	Line(bX, bY, bX + bcX, bY + bcY).draw(2, Palette::Red);
	Line(cX, cY, cX + caX, cY + caY).draw(2, Palette::Red);
}
