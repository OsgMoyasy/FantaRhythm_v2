#include "Title.h"
#include "SceneManager.h"

#define CYCLE 3000
#define TWOPI 6.28318

Title::Title(void) {
	FontAsset::Register(U"font", 50);
	FontAsset::Preload(U"font");
	TextureAsset::Register(U"back", U"resources/images/back/start_back_image.jpg");
	TextureAsset::Preload(U"back");
}

Title::~Title(void) {
	FontAsset::Unregister(U"font");
	TextureAsset::Unregister(U"back");
}

void Title::update(void) {
	if (KeyA.down()) {//‹È‘I‘ğ‰æ–Ê‚Ö
		SceneManager::setNextScene(SceneManager::SCENE_SELECTMUSIC);
	}
	else {
		changeAlpha();
	}
}

void Title::draw(void) {
	//”wŒi‰æ‘œ•`‰æ
	TextureAsset(U"back").draw();

	//•¶š—ñ•`‰æ
	FontAsset(U"font")(U"` Press Button To Start `").drawAt(Window::Width() / 2 + 3, Window::Height() - 150 + 3, ColorF(0, 0, 0, alpha - 0.05));
	FontAsset(U"font")(U"` Press Button To Start `").drawAt(Window::Width() / 2, Window::Height() - 150, AlphaF(alpha));
}

void Title::changeAlpha(void) {
	const int32 t = Time::GetMillisec();
	alpha = Sin(t % CYCLE / static_cast<double>(CYCLE) * TWOPI) * 0.42 + 0.58;
}