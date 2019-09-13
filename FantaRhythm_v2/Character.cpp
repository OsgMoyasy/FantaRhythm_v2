#include"Character.h"
#include"FantaRhythm_v2.h"

constexpr int moverange = 70;		//振幅 上下の長さは*2
constexpr int movefreq = 4 * 60;	//上下する周期	左の値を秒指定
constexpr int effectsize = 200;
bool Character::guardflag;

Character::Character(CharacterSubject* csubject, const FilePath& jobname,const CSVData &csv , double ix, double iy,int row) {
	this->csubject = csubject;
	
	flipeffect[EffectType::NOMAL] = new FlipEffect(U"resources/images/effect/"+ jobname +U"/attack.png", effectsize, effectsize, 0, 0);
	flipeffect[EffectType::ULT] = new FlipEffect(U"resources/images/effect/" + jobname + U"/ult.png", effectsize, effectsize, 0, 0);
	flipeffect[EffectType::DAMAGE] = new FlipEffect(U"resources/images/effect/" + jobname + U"/damage.png", effectsize, effectsize, 0, 0);


	chnumber = csv.get<int>(row, 0);
	name = csv.get<String>(row, 2);
	hp = csv.get<int>(row, 3);
	power = csv.get<int>(row,4);
	args1 = csv.get<int>(row, 5);
	args2 = csv.get<int>(row, 6);
	TextureAsset::Register(name,U"resources/images/character/"+name+U".png");
	TextureAsset::Preload(name);
	initx = ix;
	inity = iy;
	framecnt = 0;
}

Character::~Character() {
	TextureAsset::UnregisterAll();
}

void Character::chaDraw() {
	TextureAsset(name).drawAt(x, y);
}


void Character::moveUpDown() {
	y = inity + sin(Math::Pi * 2.0 / movefreq * framecnt++) * moverange;
	x = initx;
}

void Character::moveRigthLight() {

}

void Character::damage(int damage) {
	if (guardflag == 1) {
		//ガードエフェクト
		guardflag = false;
	}
	else {
		hp -= damage;
		playEffect(EffectType::DAMAGE, x, y);
	}
}

int Character::getHp() {
	return hp;
}

int Character::getPower() {
	return power;
}

int Character::getArgs1() {
	return args1;
}

int Character::getArgs2() {
	return args2;
}
void Character::setAttackEvent(int attack, EffectType::Type type) {
	playEffect(type);
	csubject->setEvent(attack);
	csubject->notifyObservers();//イベント起動
}

void Character::playEffect(EffectType::Type type) {
	flipeffect[type]->play((int)(x - effectsize / 3), (int)y);
}

void Character::playEffect(EffectType::Type type, double x, double y) {
	flipeffect[type]->play((int)x, (int)y);
}

void Character::drawEffect(void) {
	for (FlipEffect* feffect : flipeffect) {
		feffect->draw();
	}
}

void Character::onGuardFlag(void) {
	guardflag = true;
}

void Character::guard(void) {

}
