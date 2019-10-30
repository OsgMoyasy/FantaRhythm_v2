#include "Archer.h"
constexpr int ARROWSMAX = 5;

//�|���̃W���u
//��{�^���ŋ|��~����(�ő�5�{)
//���{�^���Ŋm���U��

Archer::Archer(CharacterSubject* csubject, String& char_name, int hp, int power, double generic1, double generic2, double ix, double iy) :Character(csubject, U"archer", char_name, hp, power, generic1, generic2, ix, iy) {
	arrowscount = 0;
	arrowsdamage = 0;
	arrowEffect = new ArrowEffect(ix,iy);
}

Archer::~Archer() {

}

void Archer::jobDraw() {
	arrowEffect->draw(getX(), getY());
}

void Archer::jobUpdate() {
	Print << U"Having arrows=" << arrowscount;			//�����Ă����̐��A���O�͉�
}

void Archer::arrowscharge() {		//��̃`���[�W(5�{�܂�)
	setAttackEvent(getPower(), EffectType::NOMAL);
	if (arrowscount < ARROWSMAX) {
		arrowscount += 1;
		arrowEffect->add();
	}
}

void Archer::arrowsClear() {		//�U��������A����̐����Ȃ�Ό��炷
	if (arrowscount > 0) {
		arrowscount -= 1;
		arrowEffect->take();
	}
}

void Archer::arrowsAttack() {
	if (arrowscount > 0) {			//��̐�������΃����_���Ń_���[�W�A�Ȃ���Ώ��1�_���[�W
		arrowsdamage = getArgs1() * Random(1, 20);
	}
	else {
		arrowsdamage = 1;
	}
	setAttackEvent(arrowsdamage, EffectType::ULT);
	arrowsClear();
}


void Archer::upEvent(void) {
	arrowscharge();
}
void Archer::downEvent(void) {
	arrowsAttack();
}
void Archer::damageEvent(void) {
	
}


ArrowEffect::ArrowEffect(double x, double y) {
	this->x = x;
	this->y = y;
	TextureAsset::Register(U"arrow", U"resources/images/effects/archer/arrow.png");
	TextureAsset::Preload(U"arrow");
}
ArrowEffect::~ArrowEffect() {
	TextureAsset::UnregisterAll();
}
void ArrowEffect::add() {
	constexpr int OFFSETBASE = 8;
	Arrow tmp;
	int size = arrows.size();	//�T�C�Y�擾

	if (size == 0) {	//0�̏ꍇ�����ɕ`��
		tmp.offsetX = 0;
		tmp.offsetY = 0;
	}
	else {
		tmp.offsetX = OFFSETBASE * (size + 2 - 1) / 2;//�؂�グ
		tmp.offsetY = OFFSETBASE * (size + 2 - 1) / 2;
		if ((size & 1) == 1) {//���ŏ㑤�ɕ`�悷�邩�����ɕ`�悷�邩
			tmp.offsetY *= -1;
		}
	}
	arrows.push_back(tmp);
}
void ArrowEffect::take() {
	arrows.pop_back();
}
void ArrowEffect::draw(double x, double y) {
	constexpr int OFFSETY = 80, OFFSETX = - 20;
	for (const auto arrow : arrows) {
		TextureAsset(U"arrow").drawAt(OFFSETX + x + arrow.offsetX, OFFSETY + y + arrow.offsetY);
	}
}