#include"Character.h"
#include"FantaRhythm_v2.h"

constexpr int MOVERANGE = 70;	//キャラの上下移動高さ
constexpr int MOVEFREQ = 4 * 60;//キャラ移動周期（フレーム数＊時間(s))
constexpr int EFFECTSIZE = 200; //エフェクトの画像サイズ
bool Character::isGuard;

Character::Character(CharacterSubject* csubject, const FilePath& jobname,const CSVData &csv , double ix, double iy,int row) {
	this->csubject = csubject;
	
	flipeffect[EffectType::NOMAL] = new FlipEffect(U"resources/images/effect/"+ jobname +U"/attack.png", EFFECTSIZE, EFFECTSIZE, 0, 0);
	flipeffect[EffectType::ULT] = new FlipEffect(U"resources/images/effect/" + jobname + U"/ult.png", EFFECTSIZE, EFFECTSIZE, 0, 0);
	flipeffect[EffectType::DAMAGE] = new FlipEffect(U"resources/images/effect/" + jobname + U"/damage.png", EFFECTSIZE, EFFECTSIZE, 0, 0);


	characterNum = csv.get<int>(row, 0);
	name = csv.get<String>(row, 2);
	hp = csv.get<int>(row, 3);
	power = csv.get<int>(row,4);
	args1 = csv.get<int>(row, 5);
	args2 = csv.get<double>(row, 6);
	TextureAsset::Register(name,U"resources/images/character/"+name+U".png");
	TextureAsset::Preload(name);
	initx = ix;
	inity = iy;
	framecnt = 0;
}

Character::~Character() {
	TextureAsset::UnregisterAll();
}

void Character::characterDraw() {
	TextureAsset(name).drawAt(x, y);
}


void Character::moveUpDown() {
	y = inity + sin(Math::Pi * 2.0 / MOVEFREQ * framecnt++) * MOVERANGE;
	x = initx;
}

void Character::moveRigthLight() {

}

void Character::damage(int damage) {
		hp -= damage;
		playEffect(EffectType::DAMAGE, x, y);
}

void Character::cheakdamage(int damage) {//ガードを使用するときに使う用
	if (isGuard == 1) {
		isGuard = false;
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

double Character::getArgs2() {
	return args2;
}

void Character::setAttackEvent(int attack, EffectType::Type type) {
	playEffect(type);
	csubject->setEvent(attack);
	csubject->notifyObservers();
}

void Character::playEffect(EffectType::Type type) {
	flipeffect[type]->play((int)(x - EFFECTSIZE / 3), (int)y);
}

void Character::playEffect(EffectType::Type type, double x, double y) {
	flipeffect[type]->play((int)x, (int)y);
}

void Character::drawEffect(void) {
	for (FlipEffect* numEffect : flipeffect) {
		numEffect->draw();
	}
}


void Character::onGuardFlag(void) {
	isGuard = true;
}

void Character::guard(void) {

}

int Character::heal() {
	return 0;
}

void Character::recovery() {
	if (heal() > 0) {
		hp += heal();
	}
}