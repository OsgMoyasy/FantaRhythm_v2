#include"Character.h"
#include"FantaRhythm_v2.h"

constexpr int MOVERANGE = 70;	//キャラの上下移動高さ
constexpr int MOVEFREQ = 4 * 60;//キャラ移動周期（フレーム数＊時間(s))
constexpr int EFFECTSIZE = 200; //エフェクトの画像サイズ

Character::Character(CharacterSubject* csubject, const FilePath& jobname, String& char_name, int hp, int power, double generic1, double generic2, double ix, double iy) {
	this->csubject = csubject;
	//エフェクトの作成
	flipeffect[EffectType::NOMAL] = new FlipEffect(U"resources/images/effects/"+ jobname +U"/attack.png", EFFECTSIZE, EFFECTSIZE, 0, 0, 0.05);
	flipeffect[EffectType::ULT] = new FlipEffect(U"resources/images/effects/" + jobname + U"/ult.png", EFFECTSIZE, EFFECTSIZE, 0, 0);
	flipeffect[EffectType::DAMAGE] = new FlipEffect(U"resources/images/effects/damage.png", EFFECTSIZE, EFFECTSIZE, 0, 0, 0.04);
	flipeffect[EffectType::GUARD] = new FlipEffect(U"resources/images/effects/shield.png", EFFECTSIZE, EFFECTSIZE, 0, 0.003);
	flipeffect[EffectType::HEAL] = new FlipEffect(U"resources/images/effects/heal.png", EFFECTSIZE, EFFECTSIZE, 0, 0.003);
	//CSVファイルの読み込み
	this->char_name = char_name;
	this->hp = hp;
	this->power = power;
	this->generic1 = generic1;
	this->generic2 = generic2;
	//キャラ画像の読み込み
	TextureAsset::Register(char_name,U"resources/images/character/R"+char_name+U".png", AssetParameter::LoadAsync());
	TextureAsset::Preload(char_name);
	initx = ix;
	inity = iy;
	framecnt = 0;
	moveUpDown();
}

Character::~Character(void) {
	TextureAsset::Unregister(char_name);
	for (int i = 0; i < EffectType::SIZE; i++) {
		delete flipeffect[i];
	}
}

bool Character::isReady(void) {
	if (TextureAsset::IsReady(char_name)) {
		return true;
	}
	return false;
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
	playEffect(EffectType::HEAL, x, y);
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
	return generic1;
}

double Character::getArgs2() {
	return generic2;
}

double Character::getX() {
	return x;
}
double Character::getY() {
	return y;
}
int Character::getW() {
	return TextureAsset(char_name).width();
}
int Character::getH() {
	return TextureAsset(char_name).height();
}
String Character::getName() {
	return char_name;
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
	TextureAsset(char_name).drawAt(x, y);
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

