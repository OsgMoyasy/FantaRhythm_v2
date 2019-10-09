#include "Enemy.h"



Enemy::Enemy(String path) :inity(Window::Height() / 2), initx(1100){
	TextureAsset::Register(U"enemy", path + U"/enemy.png");
	TextureAsset::Preload(U"enemy");

	flipeffect[EffectType::NOMAL] = new FlipEffect(U"resources/images/effects/magic.png", 200, 200, initx, inity);
	flipeffect[EffectType::ULT] = new FlipEffect(U"resources/images/effects/magic.png", 200, 200, initx, inity);
	flipeffect[EffectType::DAMAGE] = new FlipEffect(U"resources/images/effects/magic.png", 200, 200, initx, inity);
	x = initx;
	y = inity;
	framecnt = 0;
	totalDamage = 0;
}
Enemy::~Enemy(){
	TextureAsset::UnregisterAll();
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

void Enemy::funcEvent(Obj obj) {//�_���[�W���󂯂�C�x���g���
	addDamage(obj.val);
	flipeffect[EffectType::DAMAGE]->play(x, y);
}

void Enemy::attack(void) {
	flipeffect[EffectType::NOMAL]->play(x + 70, y);
}