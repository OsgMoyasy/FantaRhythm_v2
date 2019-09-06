#include "Enemy.h"

Enemy::Enemy(String path) :inity(Window::Height() / 2), initx(100){
	TextureAsset::Register(U"enemy", path + U"/enemy.png");
	TextureAsset::Preload(U"enemy");
	flipeffect = new FlipEffect(U"resources/images/effect/magic.png", 200, 200, initx, inity);
	x = initx;
	y = inity;
	framecnt = 0;
	totaldmg = 0;
}
Enemy::~Enemy(){
	TextureAsset::UnregisterAll();
}
void Enemy::update() {
	moveUpDown();
	
}
void Enemy::draw() {
	Print << U"enemydmg" + Format(totaldmg);
	TextureAsset(U"enemy").drawAt(x, y);
	flipeffect->draw();

}
void Enemy::addDamage(int dmg) {
	totaldmg += dmg;
}
int Enemy::getTotalDamage() {
	return totaldmg;
}

void Enemy::moveUpDown() {
	constexpr int movefreq = 240;
	y = inity + sin(Math::TwoPi / movefreq * framecnt++) * 100;
} 

void Enemy::funcEvent(Obj obj) {//ダメージを受けるイベント受取
	addDamage(obj.val);

	flipeffect->play(x, y);
}