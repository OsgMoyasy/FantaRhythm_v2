#include "Result.h"

#define CYCLE 3000
#define TWOPI 6.28318

Result::Result(JUDGE::JudgeCount judgecnt, int totaldmg, bool cflag) {
	judgecount = judgecnt;
	totaldamage = totaldmg;
	clearflag = cflag;

	FontAsset::Register(U"font", 50);
	FontAsset::Preload(U"font");
	FontAsset::Register(U"subfont", 30);
	FontAsset::Preload(U"subfont");
	if (clearflag) {//�Q�[���N���A
		TextureAsset::Register(U"back", U"resources/images/back/BackScreen.jpg");
		stateUpdate = &Result::successUpdate;
		stateDraw = &Result::successDraw;
		calcScore();
	}
	else {//�Q�[���I�[�o�[
		TextureAsset::Register(U"back", U"resources/images/back/gameOver.jpg");
		stateUpdate = &Result::failedUpdate;
		stateDraw = &Result::failedDraw;

		framecnt = 0;
		alpha = 0;
		alphafont = 0;
	}
	TextureAsset::Preload(U"back");//�w�i�̃��[�h
}
Result::~Result(void) {
	FontAsset::Unregister(U"font");
	FontAsset::Unregister(U"subfont");
	TextureAsset::UnregisterAll();
}
void Result::update(void) {
	(this->*stateUpdate)();
}
void Result::draw(void) {
	

	(this->*stateDraw)();
}

//�Q�[���N���A�p
void Result::calcScore(void) {//�X�R�A�v�Z
	constexpr int weight[JUDGE::TYPE_SIZE] = {100, 70, 50, 0};
	score = 0;
	for (int i = 0; i < JUDGE::TYPE_SIZE; i++) {
		score += weight[i] * judgecount.cnt[i];
	}
}
void Result::successUpdate(void) {

}
void Result::successDraw(void) {
	TextureAsset(U"back").drawAt(Window::Width() / 2, Window::Height() / 2);
	FontAsset(U"subfont")(U"Score   ::"+Format(score)).draw(Window::Width() / 2 - 100, 150,Color(0x000000));
	FontAsset(U"subfont")(U"Perfect ::" + Format(judgecount.cnt[JUDGE::PERFECT])).draw(Window::Width() / 2 - 100, 190, Color(0x000000));
	FontAsset(U"subfont")(U"Great   ::" + Format(judgecount.cnt[JUDGE::GREAT])).draw(Window::Width() / 2 - 100, 220, Color(0x000000));
	FontAsset(U"subfont")(U"Good    ::" + Format(judgecount.cnt[JUDGE::GOOD])).draw(Window::Width() / 2 - 100, 260, Color(0x000000));
	FontAsset(U"subfont")(U"Bad     ::" + Format(judgecount.cnt[JUDGE::BAD])).draw(Window::Width() / 2 - 100, 300, Color(0x000000));
}
//�Q�[���I�[�o�[�p
void Result::failedUpdate(void) {
	if (framecnt <= alphatime) {
		alpha = (double)framecnt / alphatime * 1;
		framecnt++;
	}
	else {
		//�����̕`��J�n
		const uint64 t = Time::GetMillisec();
		alphafont = Sin(t % CYCLE / static_cast<double>(CYCLE) * TWOPI) * 0.42 + 0.58;
	}
}
void Result::failedDraw(void) {
	TextureAsset(U"back").drawAt(Window::Width() / 2, Window::Height() / 2, AlphaF(alpha));//�w�i�`��
	FontAsset(U"font")(U"�` Esc�L�[�ŏI�� �`").drawAt(Window::Width() / 2, Window::Height() - 150, AlphaF(alphafont));
}