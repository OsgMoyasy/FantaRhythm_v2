#include "LoadEffect.h"

/*
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
		darkenAlpha();
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

void LoadEffect::darkenAlpha() {
	if (second / 2 >= stopwatch.sF()) {
		alpha = (double)stopwatch.sF() / (second / 2);
	}
	else {
		delete prevScene;
		prevScene = NULL;
		alpha = 1.0 - (double)(stopwatch.sF() - second / 2) / (second / 2);
	}
}
*/

LoadEffect::LoadEffect() {
	alpha = 0;
	second = 0;
	stopwatch.start();
	dark = false;
	setload = false;
	loadStanby = true;
}
LoadEffect::~LoadEffect() {

}

bool LoadEffect::isUpdate(bool isLoad) {
	if (setload) {
		if (!dark) {//暗転する前
			darkenAlpha();
		}
		else {//暗転した後
			//テクスチャロードが終わっているかどうか
			if (isLoad) {
				brightenAlpha();
			}
		}
		return true;
	}
	else {
		MyKey::setKeyLock(false);
		alpha = 0;
	}
	return false;
}
void LoadEffect::draw() {
	Rect(0, 0, Window::Width(), Window::Height()).draw(ColorF(0, 0, 0, alpha));
}

void LoadEffect::setTimer(int second) {//暗転までの時間
	MyKey::setKeyLock(true);
	this->second = second;
	loadStanby = false;
	setload = true;
	stopwatch.restart();
}

void LoadEffect::darkenAlpha() {
	if (second >= stopwatch.sF()) {
		alpha = (double)stopwatch.sF() / second;
	}
	else {
		dark = true;
		loadStanby = true;
		second = 0;
	}
}
void LoadEffect::brightenAlpha() {
	if (second >= stopwatch.sF()) {
		alpha = 1 - (double)stopwatch.sF() / second;
	}
	else {
		dark = false;
		setload = false;
		second = 0;
	}
}
bool LoadEffect::isLoadStanby() {
	if (loadStanby) {
		loadStanby = false;
		return true;
	}
	return false;
}