#include "Result.h"
#include <vector>
constexpr double NUMBER_SWTIME = 0.05;						//�����G�t�F�N�g���؂�ւ�鎞��
constexpr double NUM_MAXTIME = NUMBER_SWTIME * 9 * 1000;	//�������m�肷��܂ł̎���
constexpr int NUMIM_WIDTH = 50, NUMIM_HEIGHT = 75;
constexpr int SCOREX = 950, SCOREY = 130;
const String numberImPath = U"resources/images/items/num/num2.png";


Result::Result(JUDGE::JudgeCount judgeCnt, int totalDamage, bool isClear) {
	this->judgeCnt = judgeCnt;
	this->totalDamage = totalDamage;
	this->isClear = isClear;

	alphaBack = 0;
	alphaFont = 0;
	isEffectEnd = false;

	

	FontAsset::Register(U"resultfont", 40);
	FontAsset::Preload(U"resultfont");

	if (isClear) {//�Q�[���N���A
		client = new HttpClient();

		//�e�N�X�`��������
		TextureAsset::Register(U"resultback", U"resources/images/back/BackScreen.jpg", AssetParameter::LoadAsync());
		TextureAsset::Register(U"resultscore", U"resources/images/items/Score.png", AssetParameter::LoadAsync());
		TextureAsset::Register(U"waku", U"resources/images/items/waku.png", AssetParameter::LoadAsync());

		//���ʉ�������
		se = new SE(U"resources/musics/effects/Congratulations.mp3");

		//�N���A�p�|�C���^���蓖��
		stateUpdate = &Result::successUpdate;
		stateDraw = &Result::successDraw;

		//�X�R�A�v�Z�@������ϊ�
		score = calcScore(this->judgeCnt);
		scoreStr = Format(score);
		damageStr = Format(totalDamage);

		int char_id[4];
		RankingData::getChar_id(char_id);
		std::string str = Unicode::ToUTF8(RankingData::getName());
		th = std::thread(&HttpClient::Post, client, "user_id=" + str
			+ "&music_name="+RankingData::getMusic_name().narrow()
			+ "&score="+ scoreStr.narrow() +"&damage=" + damageStr.narrow() +
			"&character_id1=" + std::to_string(char_id[0])+
			"&character_id2=" + std::to_string(char_id[1]) +
			"&character_id3=" + std::to_string(char_id[2]) +
			"&character_id4=" + std::to_string(char_id[3])
			, "/postScore", "127.0.0.1", std::ref(th_status));


		//��������摜�ϊ�
		scoreNumEffect = new NumWithEffect(numberImPath, scoreStr, 1, SCOREX, SCOREY, NUMIM_WIDTH, NUMIM_HEIGHT, NUMBER_SWTIME);
		damageNumEffect = new NumWithEffect(numberImPath, damageStr, 1, SCOREX, SCOREY + NUMIM_HEIGHT, NUMIM_WIDTH, NUMIM_HEIGHT, NUMBER_SWTIME);
		/*
		for (auto judgeimnum : judgeImNum) {
			judgeimnum = new ImageNumber(numberImPath, NUMIM_WIDTH, NUMIM_HEIGHT);
		}
		*/

		for (int i = 0; i < JUDGE::TYPE_SIZE; i++) {
			judgeImNum[i] = new ImageNumber(numberImPath, NUMIM_WIDTH, NUMIM_HEIGHT);
		}
		
		judgeDrawRow = 0;
		judgePrevTime = 0;
	}
	else {//�Q�[���I�[�o�[
		TextureAsset::Register(U"resultback", U"resources/images/back/gameOver.jpg");
		//���ʉ�������
		se = new SE(U"resources/musics/effects/orehamou.mp3");
		//�Q�[���I�[�o�[�p�|�C���^���蓖��
		stateUpdate = &Result::failedUpdate;
		stateDraw = &Result::failedDraw;
	}
	TextureAsset::Preload(U"resultback");//�w�i�̃��[�h	
}

Result::~Result(void) {
	TextureAsset::Unregister(U"resultback");
	FontAsset::Unregister(U"resultfont");
	delete se;
	delete scoreNumEffect;
	delete damageNumEffect;

	for (int i = 0; i < JUDGE::TYPE_SIZE; i++) {
		delete judgeImNum[i];
	}

	if (th.joinable()) {
		th.join();
	}
	delete client;
}

bool Result::isReady() {
	if (TextureAsset::IsReady(U"resultback")) {
		if(isClear){
			if (
				TextureAsset::IsReady(U"resultscore") &&
				TextureAsset::IsReady(U"waku")) {
				return true;
			}
		}
		else {
			return true;
		}
	}
	return false;
}


void Result::start(void) {
	MyKey::setKeyLock(true);
	stopwatch.start();
	se->play();
}

void Result::update(void) {
	(this->*stateUpdate)();
	if (MyKey::getReturnKey()) {
		SceneManager::setNextScene(SceneManager::SCENE_TITLE);
	}
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
				if (!isEffectEnd) {
					MyKey::setKeyLock(false);
					isEffectEnd = true;
				}
				changeFontAlpha();//Esc�ŏI���̃��b�Z�[�W�\���J�n
				
			}
		}
	}
}
void Result::successDraw(void) {
	TextureAsset(U"resultback").drawAt(Window::Width() / 2, Window::Height() / 2);
	TextureAsset(U"waku").drawAt(Window::Width() / 2, Window::Height() / 2);
	TextureAsset(U"resultscore").drawAt(500 , Window::Height()/2 - 39);
	scoreNumEffect->draw();
	damageNumEffect->draw();
	for (auto judgeimnum : judgeImNum) {
		judgeimnum->draw();
	}
	FontAsset(U"resultfont")(U"�` ���^�[���L�[�Ń^�C�g���� �`").drawAt(Window::Width() / 2, Window::Height() - 120, ColorF(0.0, 0.0, 0.0, alphaFont));
}

int Result::calcScore(JUDGE::JudgeCount& jc) {//�X�R�A�v�Z ����̐��Əd�݂��|�������a���X�R�A�Ƃ���
	constexpr int weight[JUDGE::TYPE_SIZE] = { 100, 70, 50, 0 };//�d�� [perfect, great, good, bad]
	int score = 0;
	for (int i = 0; i < JUDGE::TYPE_SIZE; i++) {
		score += weight[i] * jc.cnt[i];
	}
	return score;
}

bool Result::judgeUpdate() {
	if (judgeDrawRow >= JUDGE::TYPE_SIZE) {//�S�Ēǉ����I�������
		return false;
	}
	if (stopwatch.msF() - judgePrevTime >= NUM_MAXTIME ) {//�ǉ������鎞�Ԃ������琔����ǉ�����������
		judgeImNum[judgeDrawRow]->addMulti(judgeCnt.cnt[judgeDrawRow], SCOREX, SCOREY + NUMIM_HEIGHT * (judgeDrawRow + 2));
		judgePrevTime = stopwatch.msF();
		judgeDrawRow++;
	}
	return true;
}

//�Q�[���I�[�o�[�p
void Result::failedUpdate(void) {
	constexpr int alphaBackMaxMs = 3000;//�w�i�摜�̃A���t�@�l�����ɖ߂�܂ł̎���
	//�w�i�╶�������Ԍo�߂ŕ\�������邽�߂̏���
	if (stopwatch.ms() <= alphaBackMaxMs) {
		MyKey::setKeyLock(true);
		alphaBack = (double)stopwatch.ms() / alphaBackMaxMs;
	}
	else {
		if (!isEffectEnd) {
			MyKey::setKeyLock(false);
			isEffectEnd = true;
		}
		changeFontAlpha();
	}
}

void Result::failedDraw(void) {
	TextureAsset(U"resultback").drawAt(Window::Width() / 2, Window::Height() / 2, AlphaF(alphaBack));//�w�i�`��
	FontAsset(U"resultfont")(U"�` Esc�L�[�ŏI�� �`").drawAt(Window::Width() / 2, Window::Height() - 130, AlphaF(alphaFont));
}



