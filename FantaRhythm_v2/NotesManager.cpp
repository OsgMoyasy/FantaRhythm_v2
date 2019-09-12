#include "NotesManager.h"


enum class NotesManager::NOTESTYPE {
	NORMAL,
	LONG,
	SENTINEL,
};

namespace PSHBTN {
	constexpr int NONE = 0;
	constexpr int UP = 1;
	constexpr int DOWN = 2;
	constexpr int BOTH = 3;
}

namespace JUDGE_RANGE{
	constexpr int BAD = 200;//����̍ő�͈�[ms]��2
	constexpr int GOOD = 100;//GOOD����͈�[ms]��2
	constexpr int GREAT = 25;//GREAT����͈�[ms]��2
	constexpr int PERFECT = 5;//PERFECT����͈�[ms]��2
}

struct NotesManager::Notes {
	NOTESTYPE type;
	int time;
	int longtime;
	bool display;
};

NotesManager::NotesManager(NotesSubject* sub, const String& difpath) {
//	Update* update = new Update;
//	Draw* draw = new Draw;

	TextureAsset::Register(U"note", U"resources/images/items/Nort3rd.png");
	TextureAsset::Preload(U"note");
//	effectInit();

	CSVData csv;//���ʂ̎擾�@�������z��ŊǗ� 0 ���莞��(ms) 1 �����H 2 �������[��[0-3]
	Print << difpath;

	notessubject = sub;//�T�u�W�F�N�g�̓o�^

	csv.load(difpath);//���ʂ̃��[�h
	NotesManager::Notes note;
	for (int row = 0; row < csv.rows(); row++) {
		note.time = csv.get<int>(row, 0);
		note.longtime = note.time + csv.get<int>(row, 1);
		int lane = csv.get<int>(row, 2);
		switch (csv.get<int>(row, 3)) {
			case 0:
				note.type = NOTESTYPE::NORMAL;
				break;
			case 1:
				note.type = NOTESTYPE::LONG;
				break;
		}
		note.display = true;
		notelist[lane].push_back(note);
	}
	note.type = NOTESTYPE::SENTINEL;//�ԕ��̒ǉ�
	note.time = 0;
	note.longtime = 0;
	note.display = false;

	for (int i = 0; i < JUDGE::TYPE::TYPE_SIZE; i++) {
		judgecount.cnt[i] = 0;
	}
	for (int lane = 0; lane < LANESIZE; lane++) {//���[�����Ƃɓ������Ԃ��i�[
		notelist[lane].push_back(note);//�ԕ��̐ݒu
		displayitr[lane] = checkitr[lane] = notelist[lane].begin();//�`�F�b�N�p�̃C�e���[�^����
		pressedkey[lane] = 0;
		bothtime[lane] = 0;
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

	notewidth = TextureAsset(U"note").width();
	for (int i = 0; i < LANESIZE; i++) {
		down[i] = 0;
		press[i] = 0;
	}
}
NotesManager::~NotesManager() {
	TextureAsset::UnregisterAll();
}


void NotesManager::update(void)
{
	nowtime = (int)(MusicManager::getMusicTime());
	checkAttack();
	controlJudge();
}

void NotesManager::plusItr(noteitr& itr) {
	if (itr->type != NOTESTYPE::SENTINEL)//�ԕ����ǂ�������
		itr++;
}


void NotesManager::checkAttack(void) {
	down[0] = KeyQ.down() ? PSHBTN::UP : 0;
	press[0] = KeyQ.pressed() ? PSHBTN::UP : 0;

	down[1] = KeyW.down() ? PSHBTN::UP : 0;
	press[1] = KeyW.pressed() ? PSHBTN::UP : 0;

	down[2] = KeyE.down() ? PSHBTN::UP : 0;
	press[2] = KeyE.pressed() ? PSHBTN::UP : 0;

	down[3] = KeyR.down() ? PSHBTN::UP : 0;
	press[3] = KeyR.pressed() ? PSHBTN::UP : 0;


	down[0] += KeyA.down() ? PSHBTN::DOWN : 0;
	press[0] += KeyA.pressed() ? PSHBTN::DOWN : 0;

	down[1] += KeyS.down() ? PSHBTN::DOWN : 0;
	press[1] += KeyS.pressed() ? PSHBTN::DOWN : 0;

	down[2] += KeyD.down() ? PSHBTN::DOWN : 0;
	press[2] += KeyD.pressed() ? PSHBTN::DOWN : 0;

	down[3] += KeyF.down() ? PSHBTN::DOWN : 0;
	press[3] += KeyF.pressed() ? PSHBTN::DOWN : 0;

	for (int i = 0; i < LANESIZE; i++) {
		if (down[i] == PSHBTN::BOTH && bothtime[i] == 0) {//������������Ă���Ȃ猻�ݎ����i�[
			bothtime[i] = nowtime;
		}
	}
}
JUDGE::TYPE NotesManager::judgeType(int checktime) {//����̃^�C�v��Ԃ�
	if (checktime <= JUDGE_RANGE::PERFECT) {//PERFECT
		return JUDGE::PERFECT;
	}
	else if (checktime <= JUDGE_RANGE::GREAT) {//GREAT
		return JUDGE::GREAT;
	}
	else if (checktime <= JUDGE_RANGE::GOOD) {//GOOD
		return JUDGE::GOOD;
	}
	else if (checktime <= JUDGE_RANGE::BAD) {//BAD
		return JUDGE::BAD;
	}
	else {//NONE
		return JUDGE::NONE;
	}
}

void NotesManager::controlJudge(void) {
	for (int i = 0;i < LANESIZE;i++) {
		switch (checkitr[i]->type) {
		case NOTESTYPE::NORMAL:
			judgeNormal(i);
			break;
		case NOTESTYPE::LONG:
			judgeLong(i);
			break;
		default:
			break;
		}
	}
}
void NotesManager::judgeNormal(int lane) {
	JUDGE::TYPE type = NoteisHit(checkitr[lane]->time);
	if (down[lane] && type != JUDGE::NONE) {//���莞�ԉ߂��邩����\�Ń{�^����������Ă��鎞
		return judgeEvent(type, lane);
	}
	else if(nowtime > checkitr[lane]->time + JUDGE_RANGE::BAD){
		return judgeEvent(JUDGE::BAD, lane);
	}
}
JUDGE::TYPE NotesManager::NoteisHit(int judgetime) {
	return judgeType(abs(nowtime - judgetime));
}
void NotesManager::judgeLong(int lane) {
	if (pressedkey[lane] == false && down[lane]) {
		JUDGE::TYPE type = NoteisHit(checkitr[lane]->time);
		if (type <= JUDGE::GOOD) {//���[�ŉ����ꂽ��(���̔����JUDGEenum�̕��я��Ɉˑ����Ă���)
			pressedkey[lane] = down[lane];//���̃����O�m�[�c�̔����L����
		}
	}

	if (pressedkey[lane] > 0) {//�����Ƃ��̏���
		if (press[lane] == pressedkey[lane]) {//�{�^��������
			checkitr[lane]->time = (int)(nowtime);//����ʒu�ȍ~�ŉ������~�߂�
			if (nowtime >= checkitr[lane]->longtime) {//������Ă���܂܃m�[�c�̏�[���߂�����
				return judgeLongEvent(JUDGE::BAD, lane);//�����I�Ɏ��̃m�[�c�֏������ڍs
			}
		}
		else {//������
			JUDGE::TYPE type = NoteisHit(checkitr[lane]->longtime);
			return judgeLongEvent(type, lane);
		}
	}
	
	if (nowtime >= checkitr[lane]->longtime + JUDGE_RANGE::GOOD) {//����𒴂�����
		return judgeLongEvent(JUDGE::BAD, lane);
	}
}
void NotesManager::judgeLongEvent(JUDGE::TYPE type, int lane) {
	down[lane] = pressedkey[lane];
	judgeEvent(type, lane);
	pressedkey[lane] = 0;//���肵���̂Œ������̏�Ԃ�������
	bothtime[lane] = 0;
}

void NotesManager::judgeEvent(JUDGE::TYPE type, int lane) {
	checkitr[lane]->display = false;//�f�B�X�v���C�\���I�t
	plusItr(checkitr[lane]);//����Ώۂ����ɐi�߂�
	judgecount.cnt[type]++;//������J�E���g�A�b�v
	if(type == JUDGE::BAD){
		setEvent(Massage::DAMAGE, lane);
	}
	else {
		switch (down[lane]) {
		case PSHBTN::UP:
			setEvent(Massage::UPATTACK, lane);
			break;
		case PSHBTN::DOWN:
			setEvent(Massage::DOWNATTACK, lane);
			break;
		case PSHBTN::BOTH:
			setEvent(Massage::GUARD, lane);
			break;
		}
		
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

	for(int i = 0; i < LANESIZE; i++){
		Line(laneStartX[i], laneStartY, laneJudgeX[i], laneJudgeY).draw(1, Palette::Red);	//���[���̕`��
		for (noteitr itr = displayitr[i]; itr != notelist[i].end(); itr++) {
			if (nowtime < itr->time - timeRequired)//�`��O�Ȃ�`��ł��؂�
				break;
			if (itr->display == false)
				continue;

			switch (itr->type){
			case NOTESTYPE::NORMAL:
				displayNormal(i, itr->time);
				break;
			case NOTESTYPE::LONG:
				displayLong(i, itr->time, itr->longtime);
				break;
			default:
				break;
			}
		}	
	}
//	drawAllEffect();
}

double NotesManager::getProgress(int time) {
	return (timeRequired - (time - nowtime)) / timeRequired;
}

double NotesManager::progressByAngle(double progressRate) {
	using namespace std;
	constexpr double EYE_HEIGHT = 1.0;
	constexpr double START_ANGLE = 0.4_pi;//�P�ʂ̓��W�A��(0.5�ȏ��ݒ肵�Ă͂Ȃ�Ȃ�)
	constexpr double JUDGE_ANGLE = 0.1_pi;//�P�ʂ̓��W�A��
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

/*
void NotesManager::effectInit() {
	enum USE_EFFECT {
		NORMAL,
		PARFECT
	};
	FlipEffect effectNormal(U"resources/images/effect/sol.png", 43, 43, 0, 0);
	FlipEffect effectParfect(U"resources/images/effect/magic.png", 43, 43, 0, 0);
	useFlipEffect.push_back(effectNormal);
	useFlipEffect.push_back(effectParfect);
}
void NotesManager::drawAllEffect() {
	for (auto& flip : useFlipEffect) {
		flip.draw();
	}
}
useFlipEffect[NORMAL].update();.
*/
void NotesManager::setEvent(Massage msg, int val) {
	notessubject->setEvent(msg, val);//�C�x���g�I�u�W�F�N�g�Z�b�g
	notessubject->notifyObservers();//�C�x���g�N��
}

