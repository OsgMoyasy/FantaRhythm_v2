#include "Result.h"

Result::Result(JUDGE::JudgeCount judgeCnt, int totalDamage, bool clearFlag) {
	this->judgeCnt = judgeCnt;
	this->totalDamage = totalDamage;
	this->clearFlag = clearFlag;

	framecnt = 0;
	alphaBack = 0;
	alphaFont = 0;

	FontAsset::Register(U"font", 50);
	FontAsset::Preload(U"font");
	FontAsset::Register(U"subfont", 30);
	FontAsset::Preload(U"subfont");
	if (!clearFlag) {//�Q�[���N���A
		//�e�N�X�`��������
		TextureAsset::Register(U"back", U"resources/images/back/BackScreen.jpg");
		imNumberInit();
		feffect = new FlipEffect(U"resources/images/effects/num2.png", 180, 215, 0, 0);
		//���ʉ�������
		se = new SE(U"resources/musics/effects/Congratulations.wav");
		se->play();

		stateUpdate = &Result::successUpdate;
		stateDraw = &Result::successDraw;
		int score = calcScore(this->judgeCnt);
		scoreStr = Format(score);
		scoreDraw = U"               ";
		feffect->play(100,100);
	}
	else {//�Q�[���I�[�o�[
		TextureAsset::Register(U"back", U"resources/images/back/gameOver.jpg");
		stateUpdate = &Result::failedUpdate;
		stateDraw = &Result::failedDraw;
	}
	TextureAsset::Preload(U"back");//�w�i�̃��[�h
}
Result::~Result(void) {
	FontAsset::Unregister(U"font");
	FontAsset::Unregister(U"subfont");
	TextureAsset::UnregisterAll();
}
void Result::update(void) {
	framecnt++;
	(this->*stateUpdate)();
}
void Result::draw(void) {
	(this->*stateDraw)();
}

void Result::changeFontAlpha(void) {
	constexpr int CYCLE = 3000;
	const uint64 t = Time::GetMillisec();
	alphaFont = Sin(t % CYCLE / static_cast<double>(CYCLE) * Math::TwoPi) * 0.42 + 0.58;
}

//�Q�[���N���A�p
void Result::imNumberInit() {
	constexpr int w = 180, h = 215;
	Image numberBase = Image(U"resources/images/effect/num2.png");
	int wc = numberBase.width() / w;
	int wh = numberBase.height() / h;
	for (int y = 0; y < wh; y++) {
		for (int x = 0; x < wc; x++) {
			imNumber[x + wc * y] = Texture(numberBase.clipped(x * wc, y * wh, w, h));
		}
	}
	
}

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
	FontAsset(U"subfont")(U"Perfect ::" + Format(this->judgeCnt.cnt[JUDGE::PERFECT])).draw(Window::Width() / 2 - 100, 190, Color(0x000000));
	FontAsset(U"subfont")(U"Great   ::" + Format(this->judgeCnt.cnt[JUDGE::GREAT])).draw(Window::Width() / 2 - 100, 220, Color(0x000000));
	FontAsset(U"subfont")(U"Good    ::" + Format(this->judgeCnt.cnt[JUDGE::GOOD])).draw(Window::Width() / 2 - 100, 260, Color(0x000000));
	FontAsset(U"subfont")(U"Bad     ::" + Format(this->judgeCnt.cnt[JUDGE::BAD])).draw(Window::Width() / 2 - 100, 300, Color(0x000000));
	feffect->draw();
}

void Result::scoreEffect(void) {
	constexpr int frameend = 60 * 1;//���l�����肷��܂ł̃t���[����
	constexpr int framespace = 3; //���l���ڂ�ς��t���[����

	static int wordcnt = scoreStr.size() -1;
	static int number = 0;	//�ڂ�ς���Ă��鐔��

	if (wordcnt < 0) {//�S�Ċm�肵�Ă���Ȃ�ΏI��������
		return;
	}

	if (framecnt % framespace == 0) {//0-9�܂ŊԊu�󂯂ĕύX
		number++;
		number %= 10;
		String tmp = Format(number);//�ꔭ�ŏo����l�ɗv�ύX
		scoreDraw.at(wordcnt) = tmp.at(0);
	}
	if (framecnt % frameend == 0) {//�������m�肳����
		scoreDraw.at(wordcnt) = scoreStr.at(wordcnt);
		wordcnt--;
	}
}
//�Q�[���I�[�o�[�p
void Result::failedUpdate(void) {
	//�w�i�╶�������Ԍo�߂ŕ\�������邽�߂̏���
	if (framecnt <= alphatime) {
		alphaBack = (double)framecnt / alphatime;
	}
	else {
		changeFontAlpha();
	}
}

void Result::failedDraw(void) {
	TextureAsset(U"back").drawAt(Window::Width() / 2, Window::Height() / 2, AlphaF(alphaBack));//�w�i�`��
	FontAsset(U"font")(U"�` Esc�L�[�ŏI�� �`").drawAt(Window::Width() / 2, Window::Height() - 150, AlphaF(alphaFont));
}