#include"Character.h"
#include"FantaRhythm_v2.h"

constexpr int MOVERANGE = 70;	//キャラの上下移動高さ
constexpr int MOVEFREQ = 4 * 60;//キャラ移動周期（フレーム数＊時間(s))
constexpr int EFFECTSIZE = 200; //エフェクトの画像サイズ

Character::Character(CharacterSubject* csubject, const FilePath& jobname,const CSVData &csv , double ix, double iy,int row) {
	this->csubject = csubject;
	//エフェクトの作成
	flipeffect[EffectType::NOMAL] = new FlipEffect(U"resources/images/effects/"+ jobname +U"/attack.png", EFFECTSIZE, EFFECTSIZE, 0, 0);
	flipeffect[EffectType::ULT] = new FlipEffect(U"resources/images/effects/" + jobname + U"/ult.png", EFFECTSIZE, EFFECTSIZE, 0, 0);
	flipeffect[EffectType::DAMAGE] = new FlipEffect(U"resources/images/effects/" + jobname + U"/damage.png", EFFECTSIZE, EFFECTSIZE, 0, 0);
	flipeffect[EffectType::GUARD] = new FlipEffect(U"resources/images/effects/shield.png", EFFECTSIZE, EFFECTSIZE, 0, 0, 0.1);
	//CSVファイルの読み込み
	characterNum = csv.get<int>(row, 0);
	name = csv.get<String>(row, 2);
	hp = csv.get<int>(row, 3);
	power = csv.get<int>(row,4);
	args1 = csv.get<double>(row, 5);
	args2 = csv.get<double>(row, 6);
	//キャラ画像の読み込み
	TextureAsset::Register(name,U"resources/images/character/R"+name+U".png");
	TextureAsset::Preload(name);
	initx = ix;
	inity = iy;
	framecnt = 0;
	moveUpDown();
}

Character::~Character(void) {
	TextureAsset::Unregister(name);
	for (int i = 0; i < EffectType::SIZE; i++) {
		delete flipeffect[i];
	}
}

void Character::update(void) {
	moveUpDown();
	jobUpdate();
}

void Character::draw(void) {
	characterDraw();
	jobDraw();
	drawEffect();
}

void Character::getEvent(Massage msg) {
	switch (msg) {
	case Massage::BOTHATTACK://同時押しは上攻撃
		guard();
	case Massage::UPATTACK:
		upEvent();
		break;
	case Massage::DOWNATTACK:
		downEvent();
		break;
	case Massage::DAMAGE:
		damageEvent();
		break;
	}
}



int Character::getHp() {
	return hp;
}

void Character::recovery(int amount) {
	hp += amount;
}

void Character::damage(int damage) {
		hp -= damage;
		playEffect(EffectType::DAMAGE, x, y);
}


int Character::getPower() {
	return power;
}

double Character::getArgs1() {
	return args1;
}

double Character::getArgs2() {
	return args2;
}

double Character::getX() {
	return x;
}
double Character::getY() {
	return y;
}
int Character::getW() {
	return TextureAsset(name).width();
}
int Character::getH() {
	return TextureAsset(name).height();
}
String Character::getName() {
	return name;
}

void Character::setAttackEvent(int attack, EffectType type) {
	playEffect(type);
	csubject->setEvent(attack);
	csubject->notifyObservers();
}


void Character::moveUpDown(void) {
	y = inity + sin(Math::Pi * 2.0 / MOVEFREQ * framecnt++) * MOVERANGE;
	x = initx;
}

void Character::characterDraw() {
	TextureAsset(name).drawAt(x, y);
}

void Character::playEffect(EffectType type) {
	flipeffect[type]->play((int)(x + EFFECTSIZE / 3), (int)y);
}

void Character::playEffect(EffectType type, double x, double y) {
	flipeffect[type]->play((int)x, (int)y);
}

void Character::guard(void) {
	playEffect(EffectType::GUARD, x, y);
}

void Character::drawEffect(void) {
	for (FlipEffect* feffect : flipeffect) {
		feffect->draw();
	}
}

