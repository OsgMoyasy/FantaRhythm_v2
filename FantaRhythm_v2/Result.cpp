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
		int score = calcScore(judgecount);
		score = 1192;
		scoreStr = Format(score);
		scoreDraw = U"               ";
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
int Result::calcScore(JUDGE::JudgeCount &jc) {//�X�R�A�v�Z
	constexpr int weight[JUDGE::TYPE_SIZE] = {100, 70, 50, 0};
	int score = 0;
	for (int i = 0; i < JUDGE::TYPE_SIZE; i++) {
		score += weight[i] * jc.cnt[i];
	}
	return score;
}
void Result::successUpdate(void) {
	scoreEffect();

}
void Result::successDraw(void) {
	TextureAsset(U"back").drawAt(Window::Width() / 2, Window::Height() / 2);
	FontAsset(U"subfont")(U"Score   ::"+ scoreDraw).draw(Window::Width() / 2 - 100, 150,Color(0x000000));
	FontAsset(U"subfont")(U"Perfect ::" + Format(judgecount.cnt[JUDGE::PERFECT])).draw(Window::Width() / 2 - 100, 190, Color(0x000000));
	FontAsset(U"subfont")(U"Great   ::" + Format(judgecount.cnt[JUDGE::GREAT])).draw(Window::Width() / 2 - 100, 220, Color(0x000000));
	FontAsset(U"subfont")(U"Good    ::" + Format(judgecount.cnt[JUDGE::GOOD])).draw(Window::Width() / 2 - 100, 260, Color(0x000000));
	FontAsset(U"subfont")(U"Bad     ::" + Format(judgecount.cnt[JUDGE::BAD])).draw(Window::Width() / 2 - 100, 300, Color(0x000000));
}

void Result::scoreEffect(void) {
	static int cnt = 0;
	static int wordcnt = scoreStr.size() -1;
	static int number = 0;
	constexpr int frameend = 60 * 1;//���l�����肷��܂ł̎���
	constexpr int framespace = 3;
	if (wordcnt < 0) {//�S�Ċm�肵�Ă���Ȃ�ΏI��
		return;
	}
  	cnt++;//�t���[�����J�E���g

	//�����������G�ɂȂ�Ί֐���
	if (cnt % framespace == 0) {//0-9�܂ŊԊu�󂯂ĕύX
		number++;
		if (number > 9) {
			number = 0;
		}
		String tmp = Format(number);//�v�ύX
		scoreDraw.at(wordcnt) = tmp.at(0);
	}
	if (cnt % frameend == 0) {//�t���[���������ɒB������m��
		scoreDraw.at(wordcnt) = scoreStr.at(wordcnt);
		wordcnt--;
	}


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