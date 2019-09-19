#include "Result.h"
#include <vector>
constexpr double NUMBER_SWTIME = 0.05;						//�����G�t�F�N�g���؂�ւ�鎞��
constexpr double NUM_MAXTIME = NUMBER_SWTIME * 9 * 1000;	//�������m�肷��܂ł̎���
constexpr int NUMIM_WIDTH = 50, NUMIM_HEIGHT = 75;
constexpr int SCOREX = 800, SCOREY = 130;



Result::Result(JUDGE::JudgeCount judgeCnt, int totalDamage, bool isClear) {
	this->judgeCnt = judgeCnt;
	this->totalDamage = totalDamage;
	this->isClear = isClear;
	stopwatch.start();
	framecnt = 0;
	alphaBack = 0;
	alphaFont = 0;

	FontAsset::Register(U"font", 50);
	FontAsset::Preload(U"font");
	FontAsset::Register(U"subfont", 30);
	FontAsset::Preload(U"subfont");
	if (!isClear) {//�Q�[���N���A
		//�e�N�X�`��������
		TextureAsset::Register(U"back", U"resources/images/back/result.png");
		//���ʉ�������
		se = new SE(U"resources/musics/effects/Congratulations.wav");
		se->play();
		//�N���A�p�|�C���^���蓖��
		stateUpdate = &Result::successUpdate;
		stateDraw = &Result::successDraw;
		//�X�R�A�v�Z�@������ϊ�
		score = calcScore(this->judgeCnt);
		scoreStr = Format(score);
		damageStr = Format(totalDamage);
		//��������摜�ϊ�
		imNumberInit();
	}
	else {//�Q�[���I�[�o�[
		TextureAsset::Register(U"back", U"resources/images/back/gameOver.jpg");
		//�Q�[���I�[�o�[�p�|�C���^���蓖��
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
void Result::successUpdate(void) {
	double msF = stopwatch.msF();
	//���ԂɎ��Ԃ��ŕ`�悷��ׂɕ`��I�����Ă���Ȃ�Ύ��ɐi��
	if (scoreNumEffect->update(msF) == false) {
		if (damageNumEffect->update(msF) == false) {
			if (judgeUpdate() == false) {
				//�S�Ă̕`�悪�I��
			}
		}
	}
}
void Result::successDraw(void) {
	TextureAsset(U"back").drawAt(Window::Width() / 2, Window::Height() / 2);
	scoreNumEffect->draw();
	damageNumEffect->draw();
	judgeImNum->draw();

}

int Result::calcScore(JUDGE::JudgeCount& jc) {//�X�R�A�v�Z ����̐��Əd�݂��|�������a���X�R�A�Ƃ���
	constexpr int weight[JUDGE::TYPE_SIZE] = { 100, 70, 50, 0 };//�d��
	int score = 0;
	for (int i = 0; i < JUDGE::TYPE_SIZE; i++) {
		score += weight[i] * jc.cnt[i];
	}
	return score;
}

void Result::imNumberInit() {
	judgeImNum = new ImageNumber(U"resources/images/items/num/num2.png", NUMIM_WIDTH, NUMIM_HEIGHT);
	scoreNumEffect = new NumWithEffect(U"resources/images/items/num/num2.png", scoreStr, 1, SCOREX, SCOREY, NUMIM_WIDTH, NUMIM_HEIGHT,NUMBER_SWTIME);
	damageNumEffect = new NumWithEffect(U"resources/images/items/num/num2.png", damageStr, 1, SCOREX, SCOREY + NUMIM_HEIGHT, NUMIM_WIDTH, NUMIM_HEIGHT,NUMBER_SWTIME);
}

bool Result::judgeUpdate() {
	static int row = 0;//1 = damage 2 = parfect�E�E�E
	static double prevtime = stopwatch.msF();
	if (row >= JUDGE::TYPE_SIZE) {//�S�Ēǉ����I�������
		return false;
	}
	if (stopwatch.msF() - prevtime >= NUM_MAXTIME && row < 4) {//�ǉ������鎞�Ԃ������琔����ǉ�����������
		judgeImNum->addMulti(judgeCnt.cnt[row], SCOREX, SCOREY + NUMIM_HEIGHT * (row + 2));
		prevtime = stopwatch.msF();
		row++;
	}
	return true;
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



