#include "Result.h"
#include <vector>
constexpr double NUMBER_SWTIME = 0.1;
constexpr double NUM_ENDTIME = NUMBER_SWTIME * 9 * 1000;
constexpr int NUMIM_WIDTH = 100, NUMIM_HEIGHT = 150;
constexpr int SCOREX =300, SCOREY = 200;

Result::Result(JUDGE::JudgeCount judgeCnt, int totalDamage, bool clearFlag) {
	this->judgeCnt = judgeCnt;
	this->totalDamage = totalDamage;
	this->clearFlag = clearFlag;
	stopwatch.start();
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
		//���ʉ�������
		se = new SE(U"resources/musics/effects/Congratulations.wav");
		se->play();

		stateUpdate = &Result::successUpdate;
		stateDraw = &Result::successDraw;
		int score = calcScore(this->judgeCnt);
		scoreStr = Format(score);
		scoreDraw = U"               ";
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
	imnumber = new ImageNumber(U"resources/images/items/num/num.png", NUMIM_WIDTH, NUMIM_HEIGHT);
	numEffect = new FlipEffect(U"resources/images/items/num/num.png", NUMIM_WIDTH, NUMIM_HEIGHT, 0, 0, NUMBER_SWTIME);
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
	imnumber->draw();
	numEffect->draw();
}

void Result::scoreEffect(void) {
	static int canChange = 2;
	static int wordCnt = scoreStr.size() -1;
	static double prevtime = -100;

	if (wordCnt < 0) {//�S�Ċm�肵�Ă���Ȃ�ΏI��������
		return;
	}
	if (stopwatch.msF() - prevtime >= NUM_ENDTIME) {
		if (canChange == 0) {
			imnumber->add(scoreStr.at(wordCnt) - U'0', SCOREX + wordCnt * NUMIM_WIDTH, SCOREY);
			wordCnt--;
			canChange = 2;
		}
		else if (canChange > 0) {
			numEffect->play(SCOREX + wordCnt * NUMIM_WIDTH, SCOREY);
			canChange--;
			prevtime = stopwatch.msF();
		}
	}

	//�����@�Q�[���G�t�F�N�g�Ő���������Ƃ��悤�ɕύX����@��������Ŋm��ɂ���
	/*
	if (framecnt % frameend == 0) {//�������m�肳����
		scoreDraw.at(wordCnt) = scoreStr.at(wordCnt);
		wordCnt--;
	}
	*/
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



ImageNumber::ImageNumber(FilePath path, int w, int h) {//�����ɕ���ł���摜
	Image numberBase = Image(path);
	int wc = numberBase.width() / w;
	int wh = numberBase.height() / h;
	for (int i = 0; i < 10; i++) {
		imNumber[i] = Texture(numberBase.clipped(i * w, 0, w, h));
	}
}
ImageNumber::~ImageNumber() {

}
void ImageNumber::add(int num, int x, int y) {
	NumPoint tmp;
	tmp.num = num;
	tmp.x = x;
	tmp.y = y;
	numberp.push_back(tmp);
}
void ImageNumber::draw() {
	for (int i = 0; i < numberp.size(); i++) {
		imNumber[numberp.at(i).num].drawAt(numberp.at(i).x, numberp.at(i).y);
	}
}

