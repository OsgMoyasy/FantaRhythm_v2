#include "FantaRhythm_v2.h"
#define PI 3.141592


SelectMusic::SelectMusic(void) : cursor(0) {
	TextureAsset::Register(U"back", U"resources/images/back/BackScreen.jpg");
	TextureAsset::Preload(U"back");
	TextureAsset::Register(U"title", U"resources/images/items/title.png");
	TextureAsset::Preload(U"title");

	FontAsset::Register(U"font", 30);
	FontAsset::Preload(U"font");

	//曲数の取得
	musicCount = FileSystem::DirectoryContents(U"resources/musics/main/", false).count();

	/*曲名取得*/
	String musicPath;
	for (int i = 0; i < musicCount; i++) {
		musicPath = FileSystem::DirectoryContents(U"resources/musics/main/", false).at(i);
		music.push_back(FileSystem::BaseName(musicPath));
	}

	//曲プレビューの再生
	playMusic(0);
}

SelectMusic::~SelectMusic(void) {
	TextureAsset::UnregisterAll();
	FontAsset::Unregister(U"font");
	delete audio;
}

int SelectMusic::update(void) {
	moveCursor();
	rotateMusic();
	if (KeyA.down()) {
		return TITLE;
	}
	return SELECT_MUSIC;
}

void SelectMusic::draw(void) {
	//背景画像描画
	TextureAsset(U"back").draw();

	/*曲名の描画*/
	for (int i = 0; i < 5; i++) {
		//座標の指定
		int angle = 120 + 30 * i + rotation;
		int x = 1800 + cos((angle)* PI / 180.0) * 1000;
		int y = (Window::Height() / 2) - sin((angle)* PI / 180.0) * 500;
		//描画
		TextureAsset(U"title").drawAt(x, y);
		FontAsset(U"font")(music[(cursor - 2 + i + musicCount) % musicCount]).drawAt(x, y, Color(0, 0, 0));
	}
}

void SelectMusic::moveCursor(void) {
	if (KeyUp.down()) {
		cursor == 0 ? cursor = musicCount - 1 : cursor--;
		//曲名を回転させるため角度を30度マイナス
		rotation = -30;

		//新しい曲のセット&再生
		delete audio;
		playMusic(cursor);
	}
	if (KeyDown.down()) {
		cursor == musicCount - 1 ? cursor = 0 : cursor++;
		//曲名を回転させるため角度を30度プラス
		rotation = 30;

		//新しい曲のセット&再生
		delete audio;
		playMusic(cursor);
	}
}

void SelectMusic::rotateMusic(void) {
	if (rotation < 0) {
		//既定の位置にくるまで1フレームおきに角度をプラス
		rotation += 3;
	}
	if (rotation > 0) {
		//既定の位置にくるまで1フレームおきに角度をマイナス
		rotation -= 3;
	}
}

void SelectMusic::playMusic(int musicNum) {
	audio = new Audio(U"resources/musics/main/" + music[musicNum] + U"/preview.wav");
	audio->setLoop(true);
	audio->play();
}