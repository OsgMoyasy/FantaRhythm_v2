#include "Tanker.h"
//constexpr int CHARGEMAX = 10;

//�h��O��̃L�����N�^�[
//��{�^���ōU���A���{�^���ł��U���i�U���_���[�W�������j
//���������̃R���{�ŃK�[�h���͂�

Tanker::Tanker(CharacterSubject* csubject, const CSVData& csv, double ix, double iy, int row) :Character(csubject, U"tanker", csv, ix, iy, row) {
	chargeClear();
}

Tanker::~Tanker() {

}

void Tanker::draw() {
	Character::chaDraw();
	drawEffect();
}

void Tanker::update() {
	moveUpDown();
	Print << U"sealed=" << sealedcount;
}

void Tanker::upAttack() {		//���_���[�W�@���@�V�[���h�`���[�W
	setAttackEvent(getPower(), EffectType::NOMAL);
	if (sealedcount == 0 || sealedcount == 2) {		//����̃R���{���Z
		sealedcount += 1;
	}

}

void Tanker::chargeClear() {
	sealedcount = 0;
}

void Tanker::downAttack() {
	setAttackEvent(getPower(), EffectType::ULT);
	if (sealedcount == 1 || sealedcount == 3) {		//�������̃R���{���Z
		sealedcount += 1;
	}
	if (sealedcount == 4) {
		onGuardFlag();
	}
	
}

void Tanker::getEvent(Massage msg) {
	switch (msg) {
	case Massage::UPATTACK:
		upAttack();
		break;
	case Massage::DOWNATTACK:
		downAttack();
		break;
	case Massage::DAMAGE:
		chargeClear();
		break;
	}
}

