#include "Title.h"
#include "SceneManager.h"

#define CYCLE 3000
#define TWOPI 6.28318

Title::Title(void) {
	FontAsset::Register(U"titlefont", 50);
	FontAsset::Preload(U"titlefont");
	TextureAsset::Register(U"titleback", U"resources/images/back/start_back_image.jpg");
	TextureAsset::Preload(U"titleback");
}

Title::~Title(void) {

}

void Title::start(void) {

}

void Title::update(void) {
	if (MyKey::getDecisionKey()) {//‹È‘I‘ğ‰æ–Ê‚Ö
		SceneManager::setNextScene(SceneManager::SCENE_SELECTMUSIC);
	}
	else {
		changeAlpha();
	}
}

void Title::draw(void) {
	//”wŒi‰æ‘œ•`‰æ
	TextureAsset(U"titleback").draw();

	//•¶š—ñ•`‰æ
	FontAsset(U"titlefont")(U"` Press Button To Start `").drawAt(Window::Width() / 2 + 3, Window::Height() - 150 + 3, ColorF(0, 0, 0, alpha - 0.05));
	FontAsset(U"titlefont")(U"` Press Button To Start `").drawAt(Window::Width() / 2, Window::Height() - 150, AlphaF(alpha));
}

void Title::changeAlpha(void) {
	const uint64 t = Time::GetMillisec();
	alpha = Sin(t % CYCLE / static_cast<double>(CYCLE) * TWOPI) * 0.42 + 0.58;
}