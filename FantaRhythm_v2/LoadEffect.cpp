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
		if (prevScene != NULL) {
			prevScene->update();
		}
		calcAlpha();
		return true;
	}
	MyKey::setKeyLock(false);
	alpha = 0;
	return false;
}
void LoadEffect::draw() {
	if (prevScene != NULL) {
		prevScene->draw();
	}
	Rect(0, 0, Window::Width(), Window::Height()).draw(ColorF(0,0,0,alpha));
}

void LoadEffect::setTimer(int second, Scene* prevScene) {
	MyKey::setKeyLock(true);
	this->second = second;
	stopwatch.restart();
	this->prevScene = prevScene;
}

void LoadEffect::calcAlpha() {
	if (second / 2 >= stopwatch.sF()) {
		alpha = (double)stopwatch.sF() / (second / 2);
	}
	else {
		delete prevScene;
		prevScene = NULL;
		alpha = 1.0 - (double)(stopwatch.sF() - second / 2) / (second / 2);
	}
}