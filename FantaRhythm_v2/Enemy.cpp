#include "Enemy.h"

constexpr double PI = 3.14;
constexpr int initx = 50, inity = 100;

Enemy::Enemy(String path) {
	TextureAsset::Register(U"enemy", path + U"/enemy.png");
	TextureAsset::Preload(U"enemy");

	cnt = 0;
	totaldmg = 0;
}
Enemy::~Enemy(){
	TextureAsset::UnregisterAll();
}
void Enemy::update() {
	moveUpDown();
}
void Enemy::draw() {
	TextureAsset(U"enemy").drawAt(x, y);
}
void Enemy::addDamage(int dmg) {
	totaldmg += dmg;
}
int Enemy::getTotalDamage() {
	return totaldmg;
}

void Enemy::moveUpDown() {
	y = inity + sin(3.14 * 2 / 240 * cnt++) * 100;
	x = initx;
} 