#include "Result.h"


Result::Result(JUDGE::JudgeCount* judgecnt, int totaldmg, bool cflag) {
	judgecount = judgecnt;
	totaldamage = totaldmg;
	clearflag = cflag;


	if (cflag) {//ゲームクリア用
	}
	else {//ゲームオーバー用
	}
}
Result::~Result(void) {
	TextureAsset::UnregisterAll();
}
void Result::update(void) {

}
void Result::draw(void) {

}
//ゲームクリア用
void Result::gameclearUpdate(void) {

}

void Result::gameclearDraw(void) {

}
//ゲームオーバー用
void Result::gameoverUpdate(void) {

}
void Result::gameoverDraw(void) {

}