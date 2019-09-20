#include "Result.h"
#include <vector>
constexpr double NUMBER_SWTIME = 0.05;						//�����G�t�F�N�g���؂�ւ�鎞��
constexpr double NUM_MAXTIME = NUMBER_SWTIME * 9 * 1000;	//�������m�肷��܂ł̎���
constexpr int NUMIM_WIDTH = 50, NUMIM_HEIGHT = 75;
constexpr int SCOREX = 800, SCOREY = 130;
const String numberImPath = U"resources/images/items/num/num2.png";


Result::Result(JUDGE::JudgeCount judgeCnt, int totalDamage, bool isClear) {
	this->judgeCnt = judgeCnt;
	this->totalDamage = totalDamage;
	this->isClear = isClear;
	stopwatch.start();
	alphaBack = 0;
	alphaFont = 0;

	FontAsset::Register(U"font", 50);
	FontAsset::Preload(U"font");

	if (isClear) {//�Q�[���N���A
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
	TextureAsset::UnregisterAll();
}
void Result::update(void) {
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
				changeFontAlpha();//Esc�ŏI���̃��b�Z�[�W�\���J�n
			}
		}
	}
}
void Result::successDraw(void) {
	TextureAsset(U"back").drawAt(Window::Width() / 2, Window::Height() / 2);
	scoreNumEffect->draw();
	damageNumEffect->draw();
	judgeImNum->draw();
	FontAsset(U"font")(U"�` Esc�L�[�ŏI�� �`").drawAt(Window::Width() / 2, Window::Height() - 60, ColorF(0.0, 0.0, 0.0, alphaFont));
}

int Result::calcScore(JUDGE::JudgeCount& jc) {//�X�R�A�v�Z ����̐��Əd�݂��|�������a���X�R�A�Ƃ���
	constexpr int weight[JUDGE::TYPE_SIZE] = { 100, 70, 50, 0 };//�d�� [perfect, great, good, bad]
	int score = 0;
	for (int i = 0; i < JUDGE::TYPE_SIZE; i++) {
		score += weight[i] * jc.cnt[i];
	}
	return score;
}

void Result::imNumberInit() {
	scoreNumEffect = new NumWithEffect(numberImPath, scoreStr, 1, SCOREX, SCOREY, NUMIM_WIDTH, NUMIM_HEIGHT, NUMBER_SWTIME);
	damageNumEffect = new NumWithEffect(numberImPath, damageStr, 1, SCOREX, SCOREY + NUMIM_HEIGHT, NUMIM_WIDTH, NUMIM_HEIGHT, NUMBER_SWTIME);
	judgeImNum = new ImageNumber(numberImPath, NUMIM_WIDTH, NUMIM_HEIGHT);
}

bool Result::judgeUpdate() {
	static int row = 0;//1 = damage 2 = parfect�E�E�E
	static double prevtime = stopwatch.msF();
	if (row >= JUDGE::TYPE_SIZE) {//�S�Ēǉ����I�������
		return false;
	}
	if (stopwatch.msF() - prevtime >= NUM_MAXTIME ) {//�ǉ������鎞�Ԃ������琔����ǉ�����������
		judgeImNum->addMulti(judgeCnt.cnt[row], SCOREX, SCOREY + NUMIM_HEIGHT * (row + 2));
		prevtime = stopwatch.msF();
		row++;
	}
	return true;
}

//�Q�[���I�[�o�[�p
void Result::failedUpdate(void) {
	constexpr int alphaBackMaxMs = 3000;//�w�i�摜�̃A���t�@�l�����ɖ߂�܂ł̎���
	//�w�i�╶�������Ԍo�߂ŕ\�������邽�߂̏���
	if (stopwatch.ms() <= alphaBackMaxMs) {
		alphaBack = (double)stopwatch.ms() / alphaBackMaxMs;
	}
	else {
		changeFontAlpha();
	}
}

void Result::failedDraw(void) {
	TextureAsset(U"back").drawAt(Window::Width() / 2, Window::Height() / 2, AlphaF(alphaBack));//�w�i�`��
	FontAsset(U"font")(U"�` Esc�L�[�ŏI�� �`").drawAt(Window::Width() / 2, Window::Height() - 100, AlphaF(alphaFont));
}



