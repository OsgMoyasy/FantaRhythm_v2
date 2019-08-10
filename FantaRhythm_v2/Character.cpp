#include"Character.h"
#include"FantaRhythm_v2.h"

#define PI 3.141592654
int Character::count;

Character::Character(double ix, double iy) {
	TextureAsset::Register(U"ch", U"resources/images/items/Nort3rd.png");
	TextureAsset::Preload(U"ch");
	initx = ix;
	inity = iy;
	count = 0;
}

Character::~Character() {
	TextureAsset::UnregisterAll();
}

void Character::draw() {
	TextureAsset(U"ch").drawAt(x, y);
}

void Character::update() {
	y = inity + sin(PI * 2 / 240 * count++) * 100;
	x = initx;
}
