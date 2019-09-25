#include"CharacterSet.h"
#include "Jobs.h"
#include <typeinfo.h>



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
	damage = 20;
	//HPゲージの作成
	constexpr int HPWIDTH = 400;
	constexpr int HPHEIGHT = 30;
	constexpr int HPX = 800;//HPゲージ左上の位置
	constexpr int HPY = 30;
	hpGauge = new Gauge(HPX, HPY, HPWIDTH, HPHEIGHT, totalhp, Color(Palette::Red), Color(Palette::Green));
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
	}
	enemy->draw();
	hpGauge->draw();
}



void CharacterSet::funcEvent(Obj obj) {//イベントを通達
	cha[obj.val]->getEvent(obj.msg);
	switch (obj.msg) {
	case Massage::DAMAGE://自分たち側へのダメージ
		enemy->attack();
		damageToSelves(obj.val, damage);
		break;
	case Massage::CRITICALDAMAGE:
		enemy->attack();
		damageToSelves(obj.val, damage * 2);
		break;
	}
	
	for (int i = 0; i < CHANUMBER; i++) {//回復判定 要修正
		if (typeid(cha[i]) == typeid(Healer)){//回復キャラなのか判定
			int amount = ((Healer*)cha[i])->isHeal();
			if (amount > 0) {
				for (int j = 0; j < CHANUMBER; j++) {
					cha[i]->recovery(amount);
				}
			}
		}

	}
}



int CharacterSet::getTotalDamage(void) {
	return enemy->getTotalDamage();
}



void CharacterSet::damageToSelves(int lane, int damage) {
	cha[lane]->damage(damage);
	int currenthp = getCurrentHp();

	if (currenthp > totalhp) {
		currenthp = totalhp;
	}
	else if (currenthp < 0) {
		currenthp = 0;
	}
	hpGauge->update(currenthp);
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

