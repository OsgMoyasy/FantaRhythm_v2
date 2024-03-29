#include "JudgeEffect.h"

JudgeEffect::JudgeEffect(FilePath path) {
	texture[JUDGE::PERFECT] = Texture(path + U"perfect.png");
	texture[JUDGE::GREAT] = Texture(path + U"great.png");
	texture[JUDGE::GOOD] = Texture(path + U"good.png");
	texture[JUDGE::BAD] = Texture(path + U"bad.png");
	alpha = 0;
	type = JUDGE::BAD;
}

JudgeEffect::~JudgeEffect() {


}

void JudgeEffect::setEffect(JUDGE::TYPE type) {
	this->type = type;
	stopwatch.restart();
}

void JudgeEffect::update() {
	double nowSecond = stopwatch.sF();
	if (nowSecond <= drawFixedSecond) {	//透過中
		alpha = nowSecond / drawFixedSecond;
	}
	else if(nowSecond <= drawSecond){	//透過終了してから描画時間まで
		alpha = 1;
	}
	else {
		alpha = 0;
	}
}

void JudgeEffect::draw() {
	texture[type].drawAt(Window::Width() / 2,Window::Height() - 70, AlphaF(alpha));
}