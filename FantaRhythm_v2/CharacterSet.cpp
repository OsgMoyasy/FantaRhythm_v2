#include"CharacterSet.h"
#include "Jobs.h"
constexpr int HPWIDTH = 400;
constexpr int HPHEIGHT = 30;

constexpr int HPX = 800;//HPゲージ左上の位置
constexpr int HPY = 30;

CharacterSet::CharacterSet(int save[], const String& musicpath) {
	csubject = new CharacterSubject();
	
	enemy = new Enemy(musicpath);
	csubject->addObserver(enemy);

	CSVData csv;
	csv.load(U"resources/testcharadata.csv");
	for (int lane = 0; lane < CHANUMBER; lane++) {
		int initx = 900 + lane * 90, inity = 250 + lane * 80;//初期座標の設定後で見直す
		
		switch (csv.get<int>(save[lane], 1)) {//キャラ番号の行のジョブを取得
		case JOB::SOLDIER:
			cha[lane] = new Soldier(csubject, csv, initx, inity, lane);
			break;
		case JOB::ARCHER:
			cha[lane] = new Archer(csubject, csv, initx, inity, lane);
			break;
		case JOB::WITCH:
			cha[lane] = new Witch(csubject, csv, initx, inity, lane);
			break;
		case JOB::SAGE:
			cha[lane] = new Sage(csubject, csv, initx, inity, lane);
			break;
		case JOB::BERSERKER:
			cha[lane] = new Berserker(csubject, csv, initx, inity, lane);
			break;
		case JOB::COMBOSTAR:
			cha[lane] = new Combostar(csubject, csv, initx, inity, lane);
			break;
		case JOB::TANKER:
			cha[lane] = new Tanker(csubject, csv, initx, inity, lane);
			break;
		default:
			//エラー
			break;
		}
		
	}
	totalhp = getCurrentHp();
	calchpx(totalhp);
	damage = 20;
}

CharacterSet::~CharacterSet() {
	for (int i = 0; i < CHANUMBER; i++) {
		delete cha[i];
	}
}

void CharacterSet::update() {
	for (int i = 0; i < CHANUMBER; i++) {
		cha[i]->update();
	}
	enemy->update();
}

void CharacterSet::draw() {
	for (int i = 0; i < CHANUMBER; i++) {
		cha[i]->draw();
		TotalhpDraw();
	}
	enemy->draw();
}



void CharacterSet::funcEvent(Obj obj) {//イベントを通達
	cha[obj.val]->getEvent(obj.msg);
	if (obj.msg == DAMAGE) {//自分たち側へのダメージ
		enemy->attack();
		damageToSelves(obj.val);
	}
	if (obj.msg == GUARD) {//仮同時押しイベント
		cha[obj.val]->onGuardFlag();
	}
	for (int i = 0; i < CHANUMBER; i++) {//回復判定 要修正
		cha[i]->heal();
		for (int j = 0; j < CHANUMBER; j++) {
			cha[i]->recovery();
		}
	}
}

void CharacterSet::TotalhpDraw() {		//総HP表示
	Rect(HPX, HPY, HPWIDTH, HPHEIGHT).draw(Palette::Red);
	Rect(HPX, HPY, hpx, HPHEIGHT).draw(Palette::Green);
}

int CharacterSet::getTotalDamage(void) {
	return enemy->getTotalDamage();
}

void CharacterSet::calchpx(int currenthp) {
	double per = (double)currenthp / totalhp;
	hpx = (int)(per * HPWIDTH);
}

void CharacterSet::damageToSelves(int lane) {
	cha[lane]->damage(damage);
	int currenthp = getCurrentHp();

	if (currenthp > totalhp) {
		currenthp = totalhp;
	}
	else if (currenthp < 0) {
		currenthp = 0;
	}
	calchpx(currenthp);
}

int CharacterSet::getCurrentHp(void) {
	int currenthp = 0;
	for (int lane = 0; lane < CHANUMBER; lane++) {
		currenthp += cha[lane]->getHp();
	}
	return currenthp;
}

void CharacterSet::gameEndEffect(void) {
	for (int lane = 0; lane < CHANUMBER; lane++) {
		cha[lane]->damage(0);
	}
}

