#include "Result.h"


Result::Result(JUDGE::JudgeCount* judgecnt, int totaldmg, bool cflag) {
	judgecount = judgecnt;
	totaldamage = totaldmg;
	clearflag = cflag;


	if (cflag) {//�Q�[���N���A�p
	}
	else {//�Q�[���I�[�o�[�p
	}
}
Result::~Result(void) {
	TextureAsset::UnregisterAll();
}
void Result::update(void) {

}
void Result::draw(void) {

}
//�Q�[���N���A�p
void Result::gameclearUpdate(void) {

}

void Result::gameclearDraw(void) {

}
//�Q�[���I�[�o�[�p
void Result::gameoverUpdate(void) {

}
void Result::gameoverDraw(void) {

}