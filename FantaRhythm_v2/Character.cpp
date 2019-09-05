#include"Character.h"
#include"FantaRhythm_v2.h"

constexpr int moverange = 70;		//振幅 上下の長さは*2
constexpr int movefreq = 4 * 60;	//上下する周期	左の値を秒指定
constexpr int effectsize = 200;

Character::Character(const CSVData &csv , double ix, double iy,int row, CharacterSubject* csubject, const FilePath& effectname) {
	this->csubject = csubject;
	flipeffect = new FlipEffect(U"resources/images/effect/"+ effectname +U".png", effectsize, effectsize, 0, 0);
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

void Character::damage(Obj obj) {

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
void Character::setAttackEvent(int attack) {
	playEffect();
	csubject->setEvent(attack);
	csubject->notifyObservers();//イベント起動
}

void Character::playEffect(void) {
	flipeffect->play(x - effectsize /2, y);
}

void Character::drawEffect(void) {
	flipeffect->draw();
}
