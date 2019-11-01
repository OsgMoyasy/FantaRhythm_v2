#include "AuraEffect.h"

#define EFFECT_SIZE 300

AuraEffect::AuraEffect(String jobname) {
	this->jobname = jobname;

	Image auraBase = Image(U"resources/images/effects/" + jobname + U"/aura.png");
	for (int y = 0; y < auraBase.height() / EFFECT_SIZE; y++) {
		for (int x = 0; x < auraBase.width() / EFFECT_SIZE; x++) {
			auraimage.push_back(Texture(auraBase.clipped(x * EFFECT_SIZE, y * EFFECT_SIZE, EFFECT_SIZE, EFFECT_SIZE)));
		}
	}

	isDraw = false;
	prevS = 0;
	nowIndex = 0;
	stopwatch.start();
}

AuraEffect::~AuraEffect() {
	TextureAsset::Unregister(U"aura" + jobname);
}


void AuraEffect::setFlag(bool flag) {
	stopwatch.restart();
	prevS = stopwatch.sF();
	isDraw = flag;
	nowIndex = 0;
}

void AuraEffect::update() {
	if (isDraw) {
		if (stopwatch.sF() - prevS > 0.05) {
			prevS = stopwatch.sF();
			nowIndex++;
			if (nowIndex > auraimage.size() - 1) {
				nowIndex = 0;
			}
			
		}
	}
}
void AuraEffect::draw(int x, int y) {
	if (isDraw) {
		auraimage.at(nowIndex).drawAt(x, y + 20);
	}
}
