#include "NotesManager.h"


enum class NotesManager::NOTESTYPE {
	NORMAL,
	LONG,
	CRITICAL,
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
	int judgetime;
	bool display;
};

struct NotesManager::ProPos {
	double scale;
	double x;
	double y;
};

NotesManager::NotesManager(NotesSubject* sub, const String& difpath) {
	TextureAsset::Register(U"note", U"resources/images/items/Nort3rd.png");
	TextureAsset::Preload(U"note");
	TextureAsset::Register(U"cri", U"resources/images/items/Nort2nd.png");
	TextureAsset::Preload(U"cri");

	effect.set(JUDGE::GOOD, U"resources/images/effect/nortsEffect.png", 100, 100);
	effect.set(JUDGE::GREAT, U"resources/images/effect/nortsEffect2.png", 100, 100);

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
			note.judgetime = note.time;
			break;
		case 1:
			note.type = NOTESTYPE::LONG;
			note.judgetime = note.longtime;
			break;
		case 2:
			note.type = NOTESTYPE::CRITICAL;
			note.judgetime = note.time;
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
	//�m�[�c�̑傫��
	notewidth = TextureAsset(U"note").width();
	laneStartScale = 0.2;
	laneJudgeScale = 1.0;

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
}
JUDGE::TYPE NotesManager::NoteisHit(int judgetime) {//���肷��^�C�~���O����JUDGE�̃^�C�v��Ԃ�
	return judgeType(abs(nowtime - judgetime));
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
		case NOTESTYPE::CRITICAL:
			judgeCritical(i);
			break;
		default:
			break;
		}
	}
}
void NotesManager::judgeNormal(int lane) {
	int time = checkitr[lane]->time;
	JUDGE::TYPE type = NoteisHit(time);
	if (down[lane] && type != JUDGE::NONE) {//���莞�ԉ߂��邩����\�Ń{�^����������Ă��鎞
		return judgeEvent(type, lane);
	}
	else if(nowtime > time + JUDGE_RANGE::BAD){
		return judgeEvent(JUDGE::BAD, lane);
	}
}
void NotesManager::judgeLong(int lane) {
	if (pressedkey[lane] == false && down[lane]) {
		JUDGE::TYPE type = NoteisHit(checkitr[lane]->time);
		if (type <= JUDGE::GOOD) {//���[�ŉ����ꂽ��(���̔����JUDGEenum�̕��я��Ɉˑ����Ă���)
			pressedkey[lane] = down[lane];//���̃����O�m�[�c�̔����L����
			return judgeEvent(type, lane, false);
		}
	}

	if (pressedkey[lane] > 0) {//�����Ƃ��̏���
		if ((press[lane] & pressedkey[lane]) == pressedkey[lane]) {//�{�^��������
			checkitr[lane]->time = (int)(nowtime);//����ʒu�ȍ~�ŉ������~�߂�
			if (nowtime >= checkitr[lane]->judgetime) {//������Ă���܂܃m�[�c�̏�[���߂�����
				checkitr[lane]->longtime = (int)(nowtime);//����ʒu�ȍ~�ŏ㑤���~�߂�
				if (nowtime >= checkitr[lane]->judgetime + JUDGE_RANGE::GOOD) {//�߂����Ƃ��̔������邽��
					return judgeLongEvent(JUDGE::BAD, lane);//�����I�Ɏ��̃m�[�c�֏������ڍs
				}
				
			}
		}
		else {//������
			JUDGE::TYPE type = NoteisHit(checkitr[lane]->judgetime);
			return judgeLongEvent(type, lane);
		}
	}
	
	if (nowtime >= checkitr[lane]->judgetime + JUDGE_RANGE::GOOD) {//����𒴂�����
		return judgeLongEvent(JUDGE::BAD, lane);
	}
}
void NotesManager::judgeCritical(int lane) {
	static int prevTime[LANESIZE]{ 0, 0, 0, 0 };
	static int pressHold[LANESIZE] = { 0,0,0,0 };
	static JUDGE::TYPE typeHold[LANESIZE] = { JUDGE::NONE,JUDGE::NONE,JUDGE::NONE,JUDGE::NONE };
	//judgeCriticalEvent���Ăя�L�̃��[�J���ÓI�ϐ�������������}�N��
	#define JUDGE_CRITICAL_EVENT  judgeCriticalEvent(typeHold[lane], lane, pressHold[lane]);\
								  pressHold[lane] = 0;prevTime[lane] = 0;\
								  typeHold[lane] = JUDGE::NONE;

	JUDGE::TYPE type = NoteisHit(checkitr[lane]->time);
	if (down[lane] ){//�{�^����������n�߂��ǂ����𔻒�
		if (type < JUDGE::NONE) {
			if (typeHold[lane] == JUDGE::NONE) {
				pressHold[lane] = down[lane];
				typeHold[lane] = type;
				prevTime[lane] = nowtime;
			}
		}
	}

	//�{�^���������ꂽ��̎�
	if (pressHold[lane] > 0) {
		if (press[lane] == PSHBTN::BOTH) {//���������̏ꍇ
			pressHold[lane] = PSHBTN::BOTH;
			JUDGE_CRITICAL_EVENT;//���������C�x���g
			return;
		}
		else if (press[lane] == 0 ||				//�{�^�����r���ŗ�����邩
				 nowtime - prevTime[lane] > 50) {	//������������ĂȂ��ꍇ�̏���
			JUDGE_CRITICAL_EVENT;//�ŏ��ɉ��������_�̃C�x���g���N����
			return;
		}
	}
	else {
		if (nowtime > checkitr[lane]->time + JUDGE_RANGE::BAD ||//�������Ɏ��Ԑ؂�
			typeHold[lane] == JUDGE::BAD) {//BAD����
			typeHold[lane] = JUDGE::BAD;
			JUDGE_CRITICAL_EVENT;//BAD�C�x���g
			return;
		}
	}
}


void NotesManager::judgeLongEvent(JUDGE::TYPE type, int lane) {
	down[lane] = pressedkey[lane];
	judgeEvent(type, lane);
	pressedkey[lane] = 0;//���肵���̂Œ������̏�Ԃ�������
}
void NotesManager::judgeEvent(JUDGE::TYPE type, int lane, bool next) {
	if (next) {
		noteNext(lane);
	}
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
		}
	}
}
void NotesManager::judgeCriticalEvent(JUDGE::TYPE type, int lane, int buttonType) {
	noteNext(lane);
	judgecount.cnt[type]++;//������J�E���g�A�b�v
	if (type == JUDGE::BAD) {
		setEvent(Massage::CRITICALDAMAGE, lane);
	}
	else {
		switch (buttonType) {
		case PSHBTN::UP:
			setEvent(Massage::UPATTACK, lane);
			setEvent(Massage::CRITICALDAMAGE, lane);
			break;
		case PSHBTN::DOWN:
			setEvent(Massage::DOWNATTACK, lane);
			setEvent(Massage::CRITICALDAMAGE, lane);
			break;
		case PSHBTN::BOTH:
			setEvent(Massage::BOTHATTACK, lane);
			break;
		}
	}
}
void NotesManager::noteNext(int lane) {
	checkitr[lane]->display = false;//�f�B�X�v���C�\���I�t
	plusItr(checkitr[lane]);//����Ώۂ����ɐi�߂�
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
			case NOTESTYPE::CRITICAL:
				displayCritical(i, itr->time);
				break;
			default:
				break;
			}
		}	
	}
	effect.draw();//�Đ����̑S�ẴG�t�F�N�g��`��
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
double NotesManager::getCurrentPosition(double startPos, double endPos, double progressRate) {
	return startPos + (endPos - startPos) * progressRate;
}
NotesManager::ProPos NotesManager::getProPos(int lane, int time) {
	double progressRate = progressByAngle(getProgress(time));
	double currentY = getCurrentPosition(laneStartY, laneJudgeY, progressRate);
	double currentX = getCurrentPosition(laneStartX[lane], laneJudgeX[lane], progressRate);
	double scale = getCurrentPosition(laneStartScale, laneJudgeScale, progressRate);
	return { scale ,currentX ,currentY };
}


void NotesManager::displayNormal(int lane, int time) {
	ProPos now = getProPos(lane, time);
	if (now.y > laneGoalY) {
		plusItr(displayitr[lane]);
		return;
	}
	TextureAsset(U"note").scaled(now.scale).drawAt(now.x, now.y);
}
void NotesManager::displayLong(int lane, int time, int longtime) {
	//�`��ʒu�̌v�Z
	//�㑤
	int longt = longtime < nowtime + timeRequired ? longtime : nowtime + timeRequired;
	ProPos end = getProPos(lane, longt);
	if (end.y > laneGoalY) {
		plusItr(displayitr[lane]);
		return;
	}
	//����
	ProPos bgn = getProPos(lane, time);

	//�`�揈��
	ProPos a, b = end;
	constexpr int BETW = 50;//�L�΂��_�̑����̍Čv�Z�̊Ԋu[ms]
	for (int i = longt - BETW;i > time;i -= BETW) {
		a = b;
		b = getProPos(lane, i);
		for (int linex = 0; linex <= (TextureAsset(U"note").width() / 2); linex++) {
			Line(a.x + linex * a.scale, a.y, b.x + linex * b.scale, b.y).draw(1, Color(150 + linex * 2, 50, 50));
			Line(a.x - linex * a.scale, a.y, b.x - linex * b.scale, b.y).draw(1, Color(150 + linex * 2, 50, 50));
		}
	}
	for (int linex = 0; linex <= (TextureAsset(U"note").width() / 2); linex++) {
		Line(b.x + linex * b.scale, b.y, bgn.x + linex * bgn.scale, bgn.y).draw(1, Color(150 + linex * 2, 50, 50));
		Line(b.x - linex * b.scale, b.y, bgn.x - linex * bgn.scale, bgn.y).draw(1, Color(150 + linex * 2, 50, 50));
	}

	TextureAsset(U"note").scaled(end.scale).drawAt(end.x, end.y);
	TextureAsset(U"note").scaled(bgn.scale).drawAt(bgn.x, bgn.y);
}
void NotesManager::displayCritical(int lane, int time) {
	ProPos now = getProPos(lane, time);
	if (now.y > laneGoalY) {
		plusItr(displayitr[lane]);
		return;
	}
	TextureAsset(U"cri").scaled(now.scale).drawAt(now.x, now.y);
}
void NotesManager::playNotesEffect(ProPos pos, JUDGE::TYPE type) {
	if (type <= JUDGE::GREAT) {
		effect[JUDGE::GREAT]->play(pos.x, pos.y);
		return;
	}
	if (type <= JUDGE::GOOD) {
		effect[JUDGE::GOOD]->play(pos.x, pos.y);
		return;
	}
}


void NotesManager::setEvent(Massage msg, int val) {
	notessubject->setEvent(msg, val);//�C�x���g�I�u�W�F�N�g�Z�b�g
	notessubject->notifyObservers();//�C�x���g�N��
}

