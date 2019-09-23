#include"Character.h"
#include"FantaRhythm_v2.h"

constexpr int MOVERANGE = 70;	//キャラの上下移動高さ
constexpr int MOVEFREQ = 4 * 60;//キャラ移動周期（フレーム数＊時間(s))
constexpr int EFFECTSIZE = 200; //エフェクトの画像サイズ

Character::Character(CharacterSubject* csubject, const FilePath& jobname,const CSVData &csv , double ix, double iy,int row) {
	this->csubject = csubject;
	//エフェクトの作成
	flipeffect[EffectType::NOMAL] = new FlipEffect(U"resources/images/effect/"+ jobname +U"/attack.png", EFFECTSIZE, EFFECTSIZE, 0, 0);
	flipeffect[EffectType::ULT] = new FlipEffect(U"resources/images/effect/" + jobname + U"/ult.png", EFFECTSIZE, EFFECTSIZE, 0, 0);
	flipeffect[EffectType::DAMAGE] = new FlipEffect(U"resources/images/effect/" + jobname + U"/damage.png", EFFECTSIZE, EFFECTSIZE, 0, 0);
	//CSVファイルの読み込み
	characterNum = csv.get<int>(row, 0);
	name = csv.get<String>(row, 2);
	hp = csv.get<int>(row, 3);
	power = csv.get<int>(row,4);
	args1 = csv.get<int>(row, 5);
	args2 = csv.get<int>(row, 6);
	//キャラ画像の読み込み
	TextureAsset::Register(name,U"resources/images/character/"+name+U".png");
	TextureAsset::Preload(name);
	//盾画像読み込み
	TextureAsset::Register(U"shield", U"resources/images/items/shield.png");
	TextureAsset::Preload(U"shield");
	initx = ix;
	inity = iy;
	framecnt = 0;
}

Character::~Character() {
	TextureAsset::UnregisterAll();
}

void Character::draw() {
	characterDraw();
	jobDraw();
	drawEffect();
}
void Character::update() {
	moveUpDown();
	jobUpdate();
}

void Character::characterDraw() {
	TextureAsset(name).drawAt(x, y);
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


void Character::guard(void) {
	TextureAsset(U"shield").drawAt(x, y);
}


void Character::recovery(int amount) {
	hp += amount;
}