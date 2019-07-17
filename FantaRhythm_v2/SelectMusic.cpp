#include "FantaRhythm_v2.h"
#include "SelectMusic.h"
#include "SceneManager.h"

#define PI 3.141592

#define DEFAULT_ANGLE 120
#define FORWARD_ANGLE 30
#define SPEED_ROTATION 3
#define DEFAULT_ROTATION 30

SelectMusic::SelectMusic(void)  {
	TextureAsset::Register(U"back", U"resources/images/back/BackScreen.jpg");
	TextureAsset::Preload(U"back");
	TextureAsset::Register(U"title", U"resources/images/items/title.png");
	TextureAsset::Preload(U"title");

	FontAsset::Register(U"font", 30);
	FontAsset::Preload(U"font");
	musicrotation = difrotation = 0;
	initMusic();
	initDifficulty();

	changeState(MUSIC);//初期状態を曲選択へ
	playMusic(musiccursor);//最初の曲プレビューを再生
}

SelectMusic::~SelectMusic(void) {
	delete audio;
	TextureAsset::UnregisterAll();
	FontAsset::Unregister(U"font");

}

void SelectMusic::changeState(SELECTSTATE nextstate) {
	switch (nextstate) {
	case MUSIC:
		stateUpdate = &SelectMusic::updateMusic;
		stateDraw = &SelectMusic::drawMusic;
		break;
	case DIFFICULTY:
		stateUpdate = &SelectMusic::updateDifficulty;
		stateDraw = &SelectMusic::drawDifficulty;
		break;
	case TITLE:
		SceneManager::setNextScene(SceneManager::SCENE_TITLE);
		break;
	case GAME:
		SceneManager::setNextScene(SceneManager::SCENE_GAME);
		break;

	}
}

void SelectMusic::initMusic(void) {
	musiccursor = 0;
	setArray(musicarray, U"resources/musics/main/", musiccount);
}

void SelectMusic::initDifficulty(void) {
	difcursor = 0;
	setArray(difarray, getDiffilepath(musiccursor), difcount);
}

void SelectMusic::setArray(s3d::Array<FilePath>&array,const FilePath& filepath,int& count) {
	array = FileSystem::DirectoryContents(filepath, false);
	count = array.count();
}
String SelectMusic::getDiffilepath(int cursor) {//難易度ファイルパス取得
	return (musicarray[cursor] + U"/score/");
}

void SelectMusic::update(void) {
	//状態に合わせた計算処理
	(this->*stateUpdate)();
}

void SelectMusic::updateMusic(void) {
	if (!musicrotation) {//移動処理が完了しているとき
		musicmoveCursor();
		if (KeyRight.down()) {//難易度へ
			changeState(DIFFICULTY);
		}else if (KeyLeft.down()) {//タイトルへ戻る
			changeState(TITLE);
		}
	}
	else {
		rotatemusic(musicrotation);//移動処理
	}
}
void SelectMusic::updateDifficulty(void) {
	if (!difrotation) {//移動処理が完了しているとき
		difmoveCursor();//上下移動処理
		if (KeyRight.down()) {//ゲームへ
			changeState(GAME);
		}else if (KeyLeft.down()) {//曲選択へ
			changeState(MUSIC);
		}
	}
	else {
		rotatemusic(difrotation);//移動処理
	}
}

void SelectMusic::rotatemusic(int& rotation) {
	if (rotation < 0) {
		rotation += SPEED_ROTATION;//既定の位置にくるまで1フレームおきに角度をプラス
	}else if (rotation > 0) {
		rotation -= SPEED_ROTATION;//既定の位置にくるまで1フレームおきに角度をマイナス
	}
}

void SelectMusic::draw(void) {
	//背景画像描画
	TextureAsset(U"back").draw();
	//現在の状態に合わせた選択肢の描画
	(this->*stateDraw)();
}

void SelectMusic::musicmoveCursor(void) {
	if (KeyUp.pressed() == 1 && KeyDown.pressed() == 1) {//上下両方押されてれば移動させない
		return;
	}
	if (KeyUp.pressed()) {
		musiccursor == 0 ? musiccursor = musiccount - 1 : musiccursor--;//0　～　count - 1を上方向ループ
		musicrotation = -DEFAULT_ROTATION;//選択肢を回転させるため角度を30度マイナス
		playMusic(musiccursor);
		initDifficulty();//曲に合わせた難易度へ初期化
	}
	else if (KeyDown.pressed()) {
		musiccursor == musiccount - 1 ? musiccursor = 0 : musiccursor++;
		musicrotation = DEFAULT_ROTATION;//選択肢を回転させるため角度を30度プラス
		playMusic(musiccursor);
		initDifficulty();//曲に合わせた難易度へ初期化
	}
}

void SelectMusic::difmoveCursor(void) {
	if (KeyUp.pressed() == 1 && KeyDown.pressed() == 1) {//上下両方押されてれば移動させない
		return;
	}
	if (KeyUp.pressed()) {
		difcursor == 0 ? difcursor = difcount - 1 : difcursor--;//0　～　count - 1を上方向ループ
		difrotation = -DEFAULT_ROTATION;//選択肢を回転させるため角度を30度マイナス
	}else if (KeyDown.pressed()) {
		difcursor == difcount - 1 ? difcursor = 0 : difcursor++;
		difrotation = DEFAULT_ROTATION;//選択肢を回転させるため角度を30度プラス
	}
}


void SelectMusic::playMusic(int musicNum) {
	delete audio;
	audio = new Audio(musicarray[musicNum] + U"/preview.wav");
	audio->setLoop(true);
	audio->play();
}

void SelectMusic::drawMusic(void) {
	/*曲名の描画*/
	for (int i = 0; i < 5; i++) {
		//座標の指定
		int angle = DEFAULT_ANGLE + FORWARD_ANGLE * i + musicrotation;
		int x = 1800 + cos((angle)* PI / 180.0) * 1000;
		int y = (Window::Height() / 2) - sin((angle)* PI / 180.0) * 500;
		//描画
		TextureAsset(U"title").drawAt(x, y);
		FontAsset(U"font")(FileSystem::BaseName(musicarray[(musiccursor - 2 + i + musiccount) % musiccount])).drawAt(x, y, Color(0, 0, 0));
	}
}
void SelectMusic::drawDifficulty(void) {
	/*曲名の描画*/
	for (int i = 0; i < 5; i++) {
		//座標の指定
		int angle = DEFAULT_ANGLE + FORWARD_ANGLE * i + difrotation;
		int x = 1800 + cos((angle)* PI / 180.0) * 1000;
		int y = (Window::Height() / 2) - sin((angle)* PI / 180.0) * 500;
		//描画
		TextureAsset(U"title").drawAt(x, y);
		FontAsset(U"font")(FileSystem::BaseName(difarray[(difcursor - 2 + i + difcount) % difcount])).drawAt(x, y, Color(0, 0, 0));
	}
}

String SelectMusic::getMusicPath(void) {
	return musicarray[musiccursor];
}
String SelectMusic::getDifPath(void) {
	return difarray[difcursor];
}