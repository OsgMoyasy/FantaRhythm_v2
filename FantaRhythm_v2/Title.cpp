#include "Title.h"
#define CYCLE 3000
#define TWOPI 6.28318

Title::Title(void) {
	alpha = 255;
	FontAsset::Register(U"font", 50);
	FontAsset::Preload(U"font");
	TextureAsset::Register(U"back", U"resources/images/back/start_back_image.jpg");
	TextureAsset::Preload(U"back");
}

Title::~Title(void) {
	FontAsset::Unregister(U"font");
	TextureAsset::Unregister(U"back");
}

int Title::update(void) {
	if (KeyA.down()) {
		return SELECT_MUSIC;
	}

	changeAlpha();
	return TITLE;
}

void Title::draw(void) {
	//îwåiâÊëúï`âÊ
	TextureAsset(U"back").draw();

	//ï∂éöóÒï`âÊ
	FontAsset(U"font")(U"Å` Press Button To Start Å`").drawAt(Window::Width() / 2 + 3, Window::Height() - 150 + 3, ColorF(0, 0, 0, alpha - 0.05));
	FontAsset(U"font")(U"Å` Press Button To Start Å`").drawAt(Window::Width() / 2, Window::Height() - 150, AlphaF(alpha));
}

void Title::changeAlpha(void) {
	const int32 t = Time::GetMillisec();
	alpha = Sin(t % CYCLE / static_cast<double>(CYCLE) * TWOPI) * 0.42 + 0.58;
}