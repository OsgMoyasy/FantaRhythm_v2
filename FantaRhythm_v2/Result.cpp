#include "Result.h"


Result::Result(JUDGE::JudgeCount* judgecnt, int totaldmg, bool cflag) {
	judgecount = judgecnt;
	totaldamage = totaldmg;
	clearflag = cflag;
	FontAsset::Register(U"font", 50);
	FontAsset::Preload(U"font");
	if (clearflag) {//ゲームクリア
		TextureAsset::Register(U"back", U"resources/images/back/BackScreen.jpg");
		stateUpdate = &Result::successUpdate;
		stateDraw = &Result::successDraw;
	}
	else {//ゲームオーバー
		TextureAsset::Register(U"back", U"resources/images/back/gameOver.jpg");
		stateUpdate = &Result::failedUpdate;
		stateDraw = &Result::failedDraw;

		framecnt = 0;
	}
	TextureAsset::Preload(U"back");//背景のロード
}
Result::~Result(void) {
	TextureAsset::UnregisterAll();
}
void Result::update(void) {
	(this->*stateUpdate)();
}
void Result::draw(void) {
	

	(this->*stateDraw)();
}

//ゲームクリア用
void Result::calcScore(void) {//スコア計算
	constexpr int weight[JUDGE::TYPE_SIZE] = {100, 70, 50, 0};
	//score = 
}
void Result::successUpdate(void) {

}
void Result::successDraw(void) {
	TextureAsset(U"back").drawAt(Window::Width() / 2, Window::Height() / 2);
}
//ゲームオーバー用
void Result::failedUpdate(void) {
	if (framecnt <= alphatime) {
		alpha = (double)framecnt / alphatime * 0.6;
		framecnt++;
	}
}
void Result::failedDraw(void) {
	TextureAsset(U"back").drawAt(Window::Width() / 2, Window::Height() / 2, AlphaF(alpha));//背景描画
}