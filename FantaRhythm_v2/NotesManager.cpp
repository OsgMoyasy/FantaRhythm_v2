#include "NotesManager.h"



NotesManager::NotesManager(NotesSubject* sub, const String& difpath) {
	TextureAsset::Register(U"note", U"resources/images/items/Nort3rd.png");
	TextureAsset::Preload(U"note");
	TextureAsset::Register(U"longef", U"resources/images/items/longNortsEffect2.png");
	TextureAsset::Preload(U"longef");


	CSVData csv;//���ʂ̎擾�@�������z��ŊǗ� 0 ���莞��(ms) 1 �����H 2 �������[��[0-3]
	Print << difpath;

	subject = sub;//�T�u�W�F�N�g�̓o�^

	csv.load(difpath);//���ʂ̃��[�h
	NotesManager::Notes note;
	for (int row = 0; row < csv.rows(); row++) {
		note.time = csv.get<int>(row, 0);
		note.longtime = note.time + csv.get<int>(row, 1);
		int lane = csv.get<int>(row, 2);
		switch (csv.get<int>(row, 3)) {
			case 0:
				note.type = NORMAL;
				break;
			case 1:
				note.type = LONG;
				break;
		}
		note.display = true;
		notelist[lane].push_back(note);
	}

	note.type = SENTINEL;//�ԕ��̒ǉ�
	note.time = 0;
	note.longtime = 0;
	note.display = false;

	for (int i = 0; i < JUDGE::TYPE::TYPE_SIZE; i++) {
		judgecount.cnt[i] = 0;
	}

	for (int lane = 0; lane < LANESIZE; lane++) {//���[�����Ƃɓ������Ԃ��i�[
		notelist[lane].push_back(note);//�ԕ��̐ݒu
		displayitr[lane] = checkitr[lane] = notelist[lane].begin();//�`�F�b�N�p�̃C�e���[�^����
		longflag[lane] = false;
	}

	//�`��֌W�̕ϐ��̏�����
	//X���W
	int Center = Window::Width() / 2;//���[���Q�̒��S
	int StartBetween = 80;//��[�ł̃��[���Ԃ̋���
	int JudgeBetween = 140;//������ł̃��[���Ԃ̋���
	for (int i = 0;i < LANESIZE;i++) {
		laneStartX[i] = Center + (int)((i - (LANESIZE - 1) / 2.0) * StartBetween);
		laneJudgeX[i] = Center + (int)((i - (LANESIZE - 1) / 2.0) * JudgeBetween);
	}
	//Y���W
	laneStartY = 100;
	laneJudgeY = 650;
	laneGoalY = 730;//(���Y���W���E + 10)
	//����
	notespeed = 1.0;
	timeRequired = 1500 / notespeed;
	//���ߐ�
	barBetween = 364;
	barStart = 5756;

	notewidth = TextureAsset(U"note").width();
}
NotesManager::~NotesManager() {
	TextureAsset::UnregisterAll();
}


void NotesManager::update(void)
{
	nowTime = (int)(MusicManager::getMusicTime());
	checkAttack();
	controlJudge();
}

void NotesManager::plusItr(noteitr& itr) {
	if (itr->type != SENTINEL)//�ԕ����ǂ�������
		itr++;
}


void NotesManager::checkAttack(void) {
	 down[0] = KeyA.down();
	press[0] = KeyA.pressed();
	 down[1] = KeyS.down();
	press[1] = KeyS.pressed();
	 down[2] = KeyD.down();
	press[2] = KeyD.pressed();
	 down[3] = KeyF.down();
	press[3] = KeyF.pressed();
}
void NotesManager::controlJudge(void) {
	for (int i = 0;i < LANESIZE;i++) {
		switch (checkitr[i]->type) {
		case NORMAL:
			judgeNormal(i);
			break;
		case LONG:
			judgeLong(i);
			break;
		default:
			break;
		}
	}
}
void NotesManager::judgeNormal(int lane) {
	int checktime = abs(nowTime - checkitr[lane]->time);

	if (down[lane] && checktime <= BAD_RANGE) {//������Ă邩���莞�ԓ��Ȃ画�菈��
		return judgeEvent(judgeType(checktime), lane);
	}
	else if (nowTime >= checkitr[lane]->time + BAD_RANGE) {//������ĂȂ��܂܏I����
		return judgeEvent(JUDGE::BAD, lane);
	}
}
void NotesManager::judgeLong(int lane) {
	int checktime = abs(nowTime - checkitr[lane]->time);

	if (down[lane] && checktime <= GOOD_RANGE) {//�����ꂽ��t���O�𗧂Ă�
		longflag[lane] = true;
	}

	if (longflag[lane]) {//�����Ƃ��̏���
		if (press[lane]) {//�{�^��������
			checkitr[lane]->time = (int)(nowTime);//����ʒu�ȍ~�ŉ������~�߂�
			if (nowTime >= checkitr[lane]->longtime) {//������Ă���܂ܔ���ʒu�֗�����
				checkitr[lane]->time = checkitr[lane]->longtime;//�㑤�𔻒�ʒu�Ɏ~�߂�
			}
		}
		else {//������
			checktime = abs(nowTime - checkitr[lane]->longtime);
			return judgeLongEvent(judgeType(checktime), lane);			
		}
	}
	
	if (nowTime >= checkitr[lane]->longtime + GOOD_RANGE) {//����𒴂�����
		return judgeLongEvent(JUDGE::BAD, lane);
	}
}

void NotesManager::judgeLongEvent(JUDGE::TYPE type, int lane) {
	judgeEvent(type, lane);
	longflag[lane] = false;//���肵���̂Œ������̏�Ԃ�������
}

void NotesManager::judgeEvent(JUDGE::TYPE type, int lane) {
	checkitr[lane]->display = false;//�f�B�X�v���C�\���I�t
	plusItr(checkitr[lane]);//����Ώۂ����ɐi�߂�
	judgecount.cnt[type]++;//������J�E���g�A�b�v
	if(type == JUDGE::BAD){
		setEvent(Massage::SMALLDAMAGE, lane);
	}
	else {
		setEvent(Massage::ATTACK, lane);
	}
}
JUDGE::TYPE NotesManager::judgeType(int checktime) {//����̃^�C�v��Ԃ�
	if (checktime <= PERFECT_RANGE) {//PERFECT
		return JUDGE::PERFECT;
	}
	else if (checktime <= GREAT_RANGE) {//GREAT
		return JUDGE::GREAT;
	}
	else if (checktime <= GOOD_RANGE) {//GOOD
		return JUDGE::GOOD;
	}
	else {//BAD
		return JUDGE::BAD;
	}
}

JUDGE::JudgeCount* NotesManager::getJudgeCount() {
	return &judgecount;
}




void NotesManager::draw(void){
	//�f�o�b�O�p
	for (int i = 0; i < 4; i++) {
		Print << judgecount.cnt[i];
	}
	Line(0, laneJudgeY, 1920, laneJudgeY).draw(3, Palette::Black);	//������̕`��

	//���ߐ��̕`��
	int barStartSurplus = barStart % barBetween;
	for (int i = nowTime + barBetween - ((nowTime - barStartSurplus) % barBetween);i < nowTime + 2000;i += barBetween) {
		double progressRate = progressByAngle(getProgress(i));
		double currentY = getCurrentPosition(laneStartY, laneJudgeY, progressRate);
		double currentX1 = getCurrentPosition(laneStartX[0],laneJudgeX[0],progressRate);
		double currentX2 = getCurrentPosition(laneStartX[3],laneJudgeX[3],progressRate);
		Line(currentX1, currentY, currentX2, currentY).draw(progressRate * 4, Palette::Red);
	}
//�f�o�b�O�p���ߐ�
double progressRate = progressByAngle(getProgress(5938));
double currentY = getCurrentPosition(laneStartY, laneJudgeY, progressRate);
double currentX1 = getCurrentPosition(laneStartX[0], laneJudgeX[0], progressRate);
double currentX2 = getCurrentPosition(laneStartX[3], laneJudgeX[3], progressRate);
Line(currentX1, currentY, currentX2, currentY).draw(progressRate * 4, Palette::Red);

	for(int i = 0; i < LANESIZE; i++){
		Line(laneStartX[i], laneStartY, laneJudgeX[i], laneJudgeY).draw(1, Palette::Red);	//���[���̕`��
		for (noteitr itr = displayitr[i]; itr != notelist[i].end(); itr++) {
			if (nowTime < itr->time - timeRequired)//�`��O�Ȃ�`��ł��؂�
				break;
			if (itr->display == false)
				continue;

			switch (itr->type){
			case NORMAL:
				displayNormal(i, itr->time);
				break;
			case LONG:
				displayLong(i, itr->time, itr->longtime);
				break;
			default:
				break;
			}
		}	
	}
}

double NotesManager::getProgress(int time) {
	return (timeRequired - (time - nowTime)) / timeRequired;
}

double NotesManager::progressByAngle(double progressRate) {
	using namespace std;
	constexpr double PI = 3.1415;
	constexpr double EYE_HEIGHT = 1.0;
	constexpr double START_ANGLE = 0.4 * PI;//�P�ʂ̓��W�A��(0.5�ȏ��ݒ肵�Ă͂Ȃ�Ȃ�)
	constexpr double JUDGE_ANGLE = 0.1 * PI;//�P�ʂ̓��W�A��
	const static double START_RANGE = EYE_HEIGHT * tan(START_ANGLE);
	const static double JUDGE_RANGE = EYE_HEIGHT * tan(JUDGE_ANGLE);
	double nowRange = START_RANGE - (START_RANGE - JUDGE_RANGE) * progressRate;
	return (START_ANGLE - atan(nowRange / EYE_HEIGHT)) / (START_ANGLE - JUDGE_ANGLE);
}

double NotesManager::getCurrentPosition(int startPos, int endPos, double progressRate) {
	return startPos + (endPos - startPos) * progressRate;
}

double NotesManager::getScale(double currenty) {
	double temp = currenty / (laneJudgeY - 100);//�������߂ɏk���������Ƃɖ߂����߈����Ă݂Ă���
	return  temp;
}

void NotesManager::displayNormal(int lane, int time) {
	double progressRate = progressByAngle(getProgress(time));
	double currentY = getCurrentPosition(laneStartY, laneJudgeY, progressRate);
	if (currentY > laneGoalY) {
		plusItr(displayitr[lane]);
		return;
	}
	double currentX = getCurrentPosition(laneStartX[lane], laneJudgeX[lane], progressRate);
	TextureAsset(U"note").scaled(getScale(currentY)).drawAt(currentX, currentY);
}
void NotesManager::displayLong(int lane, int time, int longtime) {
	//�`��ʒu�̌v�Z
	//�㑤
	double progressRateEnd = progressByAngle(getProgress(longtime));
	double currentEndY = getCurrentPosition(laneStartY, laneJudgeY, progressRateEnd);//�`��ʒuY���W���v�Z
	if (currentEndY > laneGoalY) {//�`�悪�I�����Ă���Ȃ�
		plusItr(displayitr[lane]);
		return;
	}
	double currentEndX = getCurrentPosition(laneStartX[lane], laneJudgeX[lane], progressRateEnd);//�`��ʒuX���W���v�Z
	if (currentEndY < laneStartY) {//�㑤���܂��`��ʒu�ɓ��B���Ă��Ȃ��Ȃ�
		currentEndX = laneStartX[lane];
		currentEndY = laneStartY;//�����ʒu�֌Œ�
	}

	//����
	double progressRateBgn = progressByAngle(getProgress(time));
	double currentBgnY = getCurrentPosition(laneStartY, laneJudgeY, progressRateBgn);//�`��ʒuY���W���v�Z
	double currentBgnX = getCurrentPosition(laneStartX[lane], laneJudgeX[lane], progressRateBgn);//�`��ʒuX���W���v�Z

	//�g�嗦�v�Z
	double scaleEnd = getScale(currentEndY);
	double scaleBgn = getScale(currentBgnY);

	//�`�揈��
	for (int linex = 0; linex <= (TextureAsset(U"note").width() / 2); linex++) {
		Line(currentEndX + linex * scaleEnd, currentEndY, currentBgnX + linex * scaleBgn, currentBgnY).draw(1, Color(150 + linex * 2, 50, 50));
		Line(currentEndX - linex * scaleEnd, currentEndY, currentBgnX - linex * scaleBgn, currentBgnY).draw(1, Color(150 + linex * 2, 50, 50));
	}
	
	TextureAsset(U"note").scaled(scaleEnd).drawAt(currentEndX, currentEndY);
	TextureAsset(U"note").scaled(scaleBgn).drawAt(currentBgnX, currentBgnY);
}

void NotesManager::setEvent(Massage msg, int val) {
	subject->setEvent(msg, val);//�C�x���g�I�u�W�F�N�g�Z�b�g
	subject->notifyObservers();//�C�x���g�N��
}
