#include "Enemy.h"



Enemy::Enemy(String path) :inity(Window::Height() / 2), initx(100){
	TextureAsset::Register(U"enemy", path + U"/enemy.png");
	TextureAsset::Preload(U"enemy");

	flipeffect[EffectType::NOMAL] = new FlipEffect(U"resources/images/effect/magic.png", 200, 200, initx, inity);
	flipeffect[EffectType::ULT] = new FlipEffect(U"resources/images/effect/magic.png", 200, 200, initx, inity);
	flipeffect[EffectType::DAMAGE] = new FlipEffect(U"resources/images/effect/magic.png", 200, 200, initx, inity);
	x = initx;
	y = inity;
	framecnt = 0;
	totalDamage = 0;
}
Enemy::~Enemy(){
	TextureAsset::Unregister(U"enemy");
	for (int i = 0; i < EffectType::SIZE; i++) {
		delete flipeffect[i];
	}
}
void Enemy::update() {
	moveUpDown();
	
}
void Enemy::draw() {
	Print << U"enemydmg" + Format(totalDamage);
	TextureAsset(U"enemy").drawAt(x, y);
	for (FlipEffect* numEffect : flipeffect) {
		numEffect->draw();
	}

}
void Enemy::addDamage(int dmg) {
	totalDamage += dmg;
}
int Enemy::getTotalDamage() {
	return totalDamage;
}

void Enemy::moveUpDown() {
	constexpr int MOVEFREQ = 240;
	y = (int)(inity + sin(Math::TwoPi / MOVEFREQ * framecnt++) * 100);
} 

void Enemy::funcEvent(Obj obj) {//ダメージを受けるイベント受取
	addDamage(obj.val);
	flipeffect[EffectType::DAMAGE]->play(x, y);
}

void Enemy::attack(void) {
	flipeffect[EffectType::NOMAL]->play(x + 70, y);
}