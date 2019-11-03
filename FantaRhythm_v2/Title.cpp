#include "Title.h"
#include "SceneManager.h"
#include"OperationTexture.h"
#include"OperationSE.h"


#define CYCLE 3000
#define TWOPI 6.28318

Title::Title(void) {
	FontAsset::Register(U"titlefont", 50);
	FontAsset::Preload(U"titlefont");
	TextureAsset::Register(U"titleback", U"resources/images/back/start_back_image.jpg", AssetParameter::LoadAsync());
}

Title::~Title(void) {
	FontAsset::Unregister(U"titlefont");
	TextureAsset::Unregister(U"titleback");
	delete TitleAudio;
}

bool Title::isReady(void) {
	if (TextureAsset::IsReady(U"titleback")) {
		return true;
	}
	return false;
}

void Title::start(void) {
	delete TitleAudio;
	TitleAudio = new Audio(U"resources/musics/main/attacking ready!/attacking ready!.wav");
	TitleAudio->setLoop(true);
	TitleAudio->play();
}


void Title::update(void) {
	if (MyKey::getDecisionKey()) {
		OperationSE::play(DECISION);
		SceneManager::setNextScene(SceneManager::SCENE_QRREAD);
	}
	else {
		changeAlpha();
	}
}

void Title::draw(void) {
	
	TextureAsset(U"titleback").draw();

	
	FontAsset(U"titlefont")(U"Å`Press Button To StartÅ`").drawAt(Window::Width() / 2 + 3, Window::Height() - 150 + 3, ColorF(0, 0, 0, alpha - 0.05));
	FontAsset(U"titlefont")(U"Å`Press Button To StartÅ`").drawAt(Window::Width() / 2, Window::Height() - 150, AlphaF(alpha));

	OperationTexture::draw(DECISION, { 10,630 }, U"ÅFêiÇﬁ");
}

void Title::changeAlpha(void) {
	const uint64 t = Time::GetMillisec();
	alpha = Sin(t % CYCLE / static_cast<double>(CYCLE) * TWOPI) * 0.42 + 0.58;
}