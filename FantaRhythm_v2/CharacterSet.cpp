#include"CharacterSet.h"
#include "Jobs.h"
#include <typeinfo>



CharacterSet::CharacterSet(const String& musicpath) {
	csubject = new CharacterSubject();
	
	enemy = new Enemy(musicpath);
	csubject->addObserver(enemy);
	
	JSONReader json(U"chardata.json");
	JSONArrayView jsonArray = json[U"user"][U"role"].arrayView();
	for (int lane = 0; lane < CHANUMBER; lane++) {
		int initx = 70 + lane * 90, inity = 250 + lane * 80;
		

		String char_name = jsonArray[lane][U"char_name"].get<String>();
		int hp = jsonArray[lane][U"hp"].get<int>();
		int power = jsonArray[lane][U"power"].get<int>();
		double generic1 = jsonArray[lane][U"generic1"].get<double>();
		double generic2 = jsonArray[lane][U"generic2"].get<double>();

		switch (jsonArray[lane][U"job_id"].get<int>()) {//�L�����ԍ��̍s�̃W���u���擾
		case JOB::SOLDIER:
			cha[lane] = new Soldier(csubject, char_name, hp, power, generic1, generic2, initx, inity);
			break;
		case JOB::ARCHER:
			cha[lane] = new Archer(csubject, char_name, hp, power, generic1, generic2, initx, inity);
			break;
		case JOB::WITCH:
			cha[lane] = new Witch(csubject, char_name, hp, power, generic1, generic2, initx, inity);
			break;
		case JOB::SAGE:
			cha[lane] = new Sage(csubject, char_name, hp, power, generic1, generic2, initx, inity);
			break;
		case JOB::BERSERKER:
			cha[lane] = new Berserker(csubject, char_name, hp, power, generic1, generic2, initx, inity);
			break;
		case JOB::COMBOSTAR:
			cha[lane] = new Combostar(csubject, char_name, hp, power, generic1, generic2, initx, inity);
			break;
			/*������
		case JOB::TANKER:
			cha[lane] = new Tanker(csubject, char_name, hp, power, generic1, generic2, initx, inity);
			break;
			*/
		default:
			//�G���[
			break;
		}
		
	}
	totalhp = getCurrentHp();
	damage = 15;
	//HP�Q�[�W�̍쐬
	constexpr int HPWIDTH = 400;
	constexpr int HPHEIGHT = 30;
	constexpr int HPX = 50;//HP�Q�[�W����̈ʒu
	constexpr int HPY = 30;
	hpGauge = new Gauge(HPX, HPY, U"resources/images/items", totalhp, Color(Palette::Red), Color(Palette::Green));
}

CharacterSet::~CharacterSet() {
	for (int i = 0; i < CHANUMBER; i++) {
		delete cha[i];
	}
	delete enemy;
	delete hpGauge;
	delete csubject;
}

bool CharacterSet::isReady(void) {
	for (int i = 0; i < CHANUMBER; i++) {
		if (!cha[i]->isReady()) {
			return false;
		}
	}
	return true;
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



void CharacterSet::funcEvent(Obj obj) {//�C�x���g��ʒB
	cha[obj.val]->getEvent(obj.msg);
	switch (obj.msg) {
	case Massage::DAMAGE://�����������ւ̃_���[�W
		enemy->attack();
		damageToSelves(obj.val, damage);
		break;
	case Massage::CRITICALDAMAGE:
		enemy->attack();
		damageToSelves(obj.val, damage * 2);
		break;
	}
	
	for (int i = 0; i < CHANUMBER; i++) {//�񕜔��� �v�C��
	
		if (Healer* p = dynamic_cast<Healer*>(cha[i])){//�񕜃L�����Ȃ̂�����
			int amount = ((Sage*)cha[i])->isHeal();
			if (amount > 0) {
				for (int j = 0; j < CHANUMBER; j++) {
					int difference = cha[j]->recovery(amount);
					amount += difference;//�����ǉ�
				}
				int currenthp = getCurrentHp();

				if (currenthp > totalhp) {
					currenthp = totalhp;
				}
				else if (currenthp < 0) {
					currenthp = 0;
				}
				hpGauge->update(currenthp);
			}
		}

	}
}



int CharacterSet::getTotalDamage(void) {
	return enemy->getTotalDamage();
}



void CharacterSet::damageToSelves(int lane, int damage) {
	cha[lane]->damage(damage);//���������HP����Ȃ��I
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

