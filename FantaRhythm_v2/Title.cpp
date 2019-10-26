#include "Title.h"
#include "SceneManager.h"

#define CYCLE 3000
#define TWOPI 6.28318

Title::Title(void) {
	FontAsset::Register(U"titlefont", 50);
	FontAsset::Preload(U"titlefont");
	TextureAsset::Register(U"titleback", U"resources/images/back/start_back_image.jpg");
	TextureAsset::Preload(U"titleback");
	httpClient = new HttpClient();
}
void Title::start(void) {
	delete TitleAudio;
	TitleAudio = new Audio(U"resources/musics/main/attacking ready!/attacking ready!.wav");
	TitleAudio->play();
}

Title::~Title(void) {
	FontAsset::Unregister(U"titlefont");
	TextureAsset::Unregister(U"titleback");
	delete TitleAudio;
	delete httpClient;
}

void Title::update(void) {
	if (MyKey::getDecisionKey()) {
		SceneManager::setNextScene(SceneManager::SCENE_QRREAD);
	}
	else {
		changeAlpha();
	}
	if (MyKey::getUpKey()) {
		httpClient->testPost("{ \"test\": \"massage\" }");
	}
}

void Title::draw(void) {
	
	TextureAsset(U"titleback").draw();

	
	FontAsset(U"titlefont")(U"Å`Press Button To StartÅ`").drawAt(Window::Width() / 2 + 3, Window::Height() - 150 + 3, ColorF(0, 0, 0, alpha - 0.05));
	FontAsset(U"titlefont")(U"Å`Press Button To StartÅ`").drawAt(Window::Width() / 2, Window::Height() - 150, AlphaF(alpha));
	//ÉeÉXÉgópèoóÕ
	Print << s3d::Unicode::Widen(httpClient->getStatusMassage());
	Print << s3d::Unicode::Widen(httpClient->getResult());
	JSONReader reader(U"test.json");
	Print << reader[U"slideshow"][U"title"].getString();
}

void Title::changeAlpha(void) {
	const uint64 t = Time::GetMillisec();
	alpha = Sin(t % CYCLE / static_cast<double>(CYCLE) * TWOPI) * 0.42 + 0.58;
}