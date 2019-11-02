#include "SelectMusic.h"
#include"OperationSE.h"
#include"OperationTexture.h"


#define DEFAULT_ANGLE 120
#define FORWARD_ANGLE 30
#define SPEED_ROTATION 3
#define DEFAULT_ROTATION 30

SelectMusic::SelectMusic(void)  {
	TextureAsset::Register(U"selectmusicback", U"resources/images/back/BackScreen.jpg", AssetParameter::LoadAsync());
	TextureAsset::Register(U"selectmusictitle", U"resources/images/items/title.png", AssetParameter::LoadAsync());

	FontAsset::Register(U"selectmusicfont", 30);
	FontAsset::Preload(U"selectmusicfont");
	musicrotation = difrotation = 0;
	initMusic();
	initDifficulty();
	changeState(MUSIC);//初期状態を曲選択へ
	notespeed = 0.8;
}

SelectMusic::~SelectMusic(void) {
	delete audio;
	FontAsset::Unregister(U"selectmusicfont");
	TextureAsset::Unregister(U"selectmusicback");
	TextureAsset::Unregister(U"selectmusictitle");
}

bool SelectMusic::isReady(void) {
	if (TextureAsset::IsReady(U"selectmusictitle") &&
		TextureAsset::IsReady(U"selectmusicback")) {
		return true;
	}
	return false;
}

void SelectMusic::start(void) {
	playMusic(musiccursor);//最初の曲プレビューを再生
}

void SelectMusic::update(void) {
	//状態に合わせた計算処理
	(this->*stateUpdate)();
}

void SelectMusic::draw(void) {
	//背景画像描画
	TextureAsset(U"selectmusicback").draw();
	//現在の状態に合わせた選択肢の描画
	(this->*stateDraw)();
	OperationTexture::draw(DECISION,  { 10,500 }, U"：選択");
	OperationTexture::draw(CANCEL,    { 10,550 }, U"：戻る");
	OperationTexture::draw(UPDOWN,    { 10,600 }, U"：曲切り替え");
	OperationTexture::draw(LEFTRIGHT, { 10,650 }, U"：ノーツスピード増減  {:.1f}"_fmt(notespeed));
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
	case QRREAD:
		SceneManager::setNextScene(SceneManager::SCENE_QRREAD);
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
	count = (int)array.count();
}
String SelectMusic::getDiffilepath(int cursor) {//難易度ファイルパス取得
	return (musicarray[cursor] + U"/score/");
}

void SelectMusic::updateMusic(void) {
	if (!musicrotation) {//移動処理が完了しているとき
		musicmoveCursor();
		if (MyKey::getDecisionKey()) {//難易度へ
			OperationSE::play(LEVER);
			changeState(DIFFICULTY);
		}else if (MyKey::getReturnKey()) {//タイトルへ戻る
			OperationSE::play(CANCEL);
			changeState(QRREAD);
		}
	}
	else {
		rotatemusic(musicrotation);//移動処理
	}
}
void SelectMusic::updateDifficulty(void) {
	if (!difrotation) {//移動処理が完了しているとき
		difmoveCursor();//上下移動処理
		if (MyKey::getDecisionKey()) {//ゲームへ
			OperationSE::play(DECISION);
			changeState(GAME);
		}else if (MyKey::getReturnKey()) {//曲選択へ
			OperationSE::play(CANCEL);
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

void SelectMusic::musicmoveCursor(void) {
	if (MyKey::getUpKey() == 1 && MyKey::getDownKey() == 1) {//上下両方押されてれば移動させない
		return;
	}
	if (MyKey::getUpKey()) {
		OperationSE::play(LEVER);
		musiccursor == 0 ? musiccursor = musiccount - 1 : musiccursor--;//0　～　count - 1を上方向ループ
		musicrotation = -DEFAULT_ROTATION;//選択肢を回転させるため角度を30度マイナス
		playMusic(musiccursor);
		initDifficulty();//曲に合わせた難易度へ初期化
	}
	else if (MyKey::getDownKey()) {
		OperationSE::play(LEVER);
		musiccursor == musiccount - 1 ? musiccursor = 0 : musiccursor++;
		musicrotation = DEFAULT_ROTATION;//選択肢を回転させるため角度を30度プラス
		playMusic(musiccursor);
		initDifficulty();//曲に合わせた難易度へ初期化
	}

	static bool pressed = false;
	if (MyKey::getLeftKey()) {
		if (notespeed > 0.6)
			notespeed -= 0.1;
		pressed = true;
	} else if (MyKey::getRightKey()) {
		if (notespeed < 2.0)
			notespeed += 0.1;
		pressed = true;
	} else {
		pressed = false;
	}
}

void SelectMusic::difmoveCursor(void) {
	if (MyKey::getUpKey() == 1 && MyKey::getDownKey() == 1) {//上下両方押されてれば移動させない
		return;
	}
	if (MyKey::getUpKey()) {
		OperationSE::play(LEVER);
		difcursor == 0 ? difcursor = difcount - 1 : difcursor--;//0　～　count - 1を上方向ループ
		difrotation = -DEFAULT_ROTATION;//選択肢を回転させるため角度を30度マイナス
	}else if (MyKey::getDownKey()) {
		OperationSE::play(LEVER);
		difcursor == difcount - 1 ? difcursor = 0 : difcursor++;
		difrotation = DEFAULT_ROTATION;//選択肢を回転させるため角度を30度プラス
	}

	static bool pressed = false;
	if (MyKey::getLeftKey()) {
		if (notespeed > 0.6)
			notespeed -= 0.1;
		pressed = true;
	} else if (MyKey::getRightKey()) {
		if (notespeed < 2.0)
			notespeed += 0.1;
		pressed = true;
	} else {
		pressed = false;
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
		int x = (int)(1800 + cos((angle)* Math::Pi / 180.0) * 1000);
		int y = (int)((Window::Height() / 2) - sin((angle)* Math::Pi / 180.0) * 500);
		//描画
		TextureAsset(U"selectmusictitle").drawAt(x, y);
		FontAsset(U"selectmusicfont")(FileSystem::BaseName(musicarray[(musiccursor - 2 + i + musiccount) % musiccount])).drawAt(x, y, Color(0, 0, 0));
		
		s3d::Array<FilePath> dif;
		int difcnt;
		setArray(dif, getDiffilepath((musiccursor - 2 + i + musiccount) % musiccount), difcnt);
		
		Rect(x - 230, y - 30, 20, 10).draw(Palette::Green);			//難易度の付け
		Rect(x - 230, y - 20, 20, 10).draw(Palette::Orange);
		if (difcnt == 3) {
			Rect(x - 230, y - 10, 20, 10).draw(Palette::Red);
		}
	}
}
void SelectMusic::drawDifficulty(void) {
	/*曲名の描画*/
	for (int i = 0; i < 5; i++) {
		//座標の指定
		int angle = DEFAULT_ANGLE + FORWARD_ANGLE * i + difrotation;
		int x = (int)(1800 + cos((angle)* Math::Pi / 180.0) * 1000);
		int y = (int)((Window::Height() / 2) - sin((angle)* Math::Pi / 180.0) * 500);
		//描画
		TextureAsset(U"selectmusictitle").drawAt(x, y);
		FontAsset(U"selectmusicfont")(FileSystem::BaseName(difarray[(difcursor - 2 + i + difcount) % difcount])).drawAt(x, y, Color(0, 0, 0));
	}
}

String SelectMusic::getMusicPath(void) {
	return musicarray[musiccursor];
}
String SelectMusic::getDifPath(void) {
	return difarray[difcursor];
}
float SelectMusic::getNoteSpeed(void) {
	return notespeed;
}