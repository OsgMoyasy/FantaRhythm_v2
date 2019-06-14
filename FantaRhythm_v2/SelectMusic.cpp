#include "FantaRhythm_v2.h"


SelectMusic::SelectMusic(void) {
	TextureAsset::Register(U"back", U"resources/images/back/BackScreen.jpg.");
	TextureAsset::Preload(U"back");
}

SelectMusic::~SelectMusic(void) {
	TextureAsset::UnregisterAll();
}

int SelectMusic::update(void) {
	if (KeyA.down()) {
		return TITLE;
	}

	return SELECT_MUSIC;
}

void SelectMusic::draw(void) {


	//”wŒi‰æ‘œ•`‰æ
	TextureAsset(U"back").draw();

}