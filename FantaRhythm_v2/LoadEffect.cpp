#include "LoadEffect.h"

LoadEffect::LoadEffect() {
	alpha = 0;
	second = 0;
	stopwatch.start();
}
LoadEffect::~LoadEffect() {

}
bool LoadEffect::isUpdate() {
	if (second >= stopwatch.sF()) {
		calsAlpha();
		return true;
	}
	alpha = 0;
	return false;
}
void LoadEffect::draw() {
	Rect(0, 0, Window::Width(), Window::Height()).draw(ColorF(0,0,0,alpha));
}

void LoadEffect::setTimer(int second) {
	this->second = second;
	stopwatch.restart();
}

void LoadEffect::calsAlpha() {
	alpha = 1.0 - (double)stopwatch.sF() / second;
}