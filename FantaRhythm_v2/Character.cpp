#include"Character.h"
#include"FantaRhythm_v2.h"

#define PI 3.141592654
constexpr int moverange = 150;		//U• ã‰º‚Ì’·‚³‚Í*2
constexpr int movefreq = 4 * 60;	//ã‰º‚·‚éüŠú	¶‚Ì’l‚ğ•bw’è

Character::Character(CSVData &csv , double ix, double iy,int row) {

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
	y = inity + sin(PI * 2.0 / movefreq * framecnt++) * moverange;
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
