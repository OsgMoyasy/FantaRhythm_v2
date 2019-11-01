#include "GameEffect.h"

//SE///////////////////////////////////////////////////////////////////////////////////
SE::SE(const FilePath& path) {
	exist = false;

	if (path != U"") {
		sound = new Audio(path);
		exist = true;
	}
}
SE::~SE() {
	delete sound;
}
void SE::play() {
	if (exist) {
		sound->stop();
		sound->play();
	}
}

//MapFlip///////////////////////////////////////////////////////////////////////////////////
MapFlip::MapFlip(Texture map, int xFlipWidth, int yFlipHeight) {
	this->map = map;
	this->xFlipWidth = xFlipWidth;
	this->yFlipHeight = yFlipHeight;
	xMapWidth = map.width();	//大きい画僧の大きさを取得
	yMapHeight = map.height();	//
	xNowPos = 0;
	yNowPos = 0;
}
bool MapFlip::nextFlip() {
	if (xNowPos += xFlipWidth, xNowPos >= xMapWidth) {	//次の列へ
		xNowPos = 0;
		if (yNowPos += yFlipHeight, yNowPos >= yMapHeight) {	//次の行の一番左の列へ
			return false;	//大きい画像の右下(最後)まで達していたらfalse
		}
	}
	return true;
}
TextureRegion MapFlip::getFlip() {
	return map(xNowPos, yNowPos, xFlipWidth, yFlipHeight);	//切り出し画像を返す
}

//FlipMovie///////////////////////////////////////////////////////////////////////////////////
FlipMovie::FlipMovie(Texture map, int xFlipWidth, int yFlipHeight, int xDraw, int yDraw, double switchBetween) {
	mapflip = new MapFlip(map, xFlipWidth, yFlipHeight);
	this->xDraw = xDraw;
	this->yDraw = yDraw;
	switchTime = switchBetween;
	this->switchBetween = switchBetween;//切り出し画像を切り替える間隔[秒?]
}


FlipMovie::~FlipMovie() {
	delete mapflip;
}
bool FlipMovie::update(double t) {
	if (switchTime < t) {				//切り替え時間がきたら
		switchTime += switchBetween;
		if (!mapflip->nextFlip()) {	//次の画僧に切り替え、大きい画像の最後まで描画していたなら
			return false;			//エフェクト再生終了
		}
	}
	mapflip->getFlip().drawAt(xDraw, yDraw);	//切り出し画像の描画
	return true;	//再生継続
}


//FlipEffect/////////////////////////////////////////////////////////////////////////////////////////////
FlipEffect::FlipEffect(const FilePath& path, int xFlipWidth, int yFlipHeight, int xDraw, int yDraw, double switchBetween)
	:map(path) {
	this->xFlipWidth = xFlipWidth;
	this->yFlipHeight = yFlipHeight;
	this->xDraw = xDraw;
	this->yDraw = yDraw;
	this->switchBetween = switchBetween;
	existSE = false;
}
FlipEffect::~FlipEffect() {
	delete se;
}

void FlipEffect::setTexture(const FilePath& path, int xFlipWidth, int yFlipHeight) {
	Texture tmp(path);
	map = tmp;
	this->xFlipWidth = xFlipWidth;
	this->yFlipHeight = yFlipHeight;
}
void FlipEffect::setPos(int xDraw, int yDraw) {
	this->xDraw = xDraw;
	this->yDraw = yDraw;
}
void FlipEffect::setSE(const FilePath& path) {
	delete se;
	se = new SE(path);
	existSE = true;
}
void FlipEffect::play() {
	effect.add<FlipMovie>(map, xFlipWidth, yFlipHeight, xDraw, yDraw, switchBetween);
	if (existSE) { se->play(); }
}
void FlipEffect::play(int xDraw, int yDraw) {
	effect.add<FlipMovie>(map, xFlipWidth, yFlipHeight, xDraw, yDraw, switchBetween);
	if (existSE) { se->play(); }
}
void FlipEffect::draw() {
	effect.update();
}

//Fractal/////////////////////////////////////////////////////////////////////////////////////////////
/*
Fractal::Fractal(int size, int x, int y) {
	constexpr int angle = 30;
	constexpr double PI = 3.1415;
	xFirst = x;
	yFirst = y;
	firstSize = size;
	x2 = size * cos(angle * (PI / 180));
	y2 = size * cos((90 - angle) * (PI / 180));
}
bool Fractal::update(double t) {
	if (t > 5) {return false;}
	drawTriangle(t, xFirst, yFirst, firstSize, x2, y2);
	return true;
}
void Fractal::drawTriangle(double t, int xPos, int yPos, int size, int x2, int y2) {
	//コードが汚い(趣味で書いただけなので後回し)
	if (t > 1) {
		drawTriangle(t - 1, xPos, yPos - size / 2, size / 2, x2 / 2, y2 / 2);
		drawTriangle(t - 1, xPos - x2 / 2, yPos + y2 / 2, size / 2, x2 / 2, y2 / 2);
		drawTriangle(t - 1, xPos + x2 / 2, yPos + y2 / 2, size / 2, x2 / 2, y2 / 2);
		t = 1.0;
	}
	int xA = xPos;
	int yA = yPos - size;
	int xB = xPos - x2;
	int yB = yPos + y2;
	int xC = xPos + x2;
	int yC = yPos + y2;
	int xAB = (-x2) * t;
	int yAB = (size + y2) * t;
	int xBC = (x2 * 2) * t;
	int yBC = 0;
	int xCA = (-x2) * t;
	int yCA = (-y2 - size) * t;
	Line(xA, yA, xA + xAB, yA + yAB).draw(2, Palette::Red);
	Line(xB, yB, xB + xBC, yB + yBC).draw(2, Palette::Red);
	Line(xC, yC, xC + xCA, yC + yCA).draw(2, Palette::Red);
}
*/

