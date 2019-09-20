#include "Game.h"

Game::Game(const String& music, const String& dif) {
	musicpath = music;
	difpath = dif;

	notessubject = new NotesSubject();
	
	notes = new NotesManager(notessubject,difpath);
	
	
	int save[4] = { 0,0,1,1 };//テスト用セーブデータ
	characterm = new CharacterSet(save,musicpath);

	notessubject->addObserver(characterm);//オブザーバーへ登録
	
	isClear = true;//falseになればゲームオーバー
	TextureAsset::Register(U"back", U"resources/images/back/first.jpg");
	TextureAsset::Preload(U"back");

	FontAsset::Register(U"font", 30);
	FontAsset::Preload(U"font");


	MusicManager::playMusicGame(musicpath);

	
}
Game::~Game() {
	delete notes;
	TextureAsset::UnregisterAll();
	FontAsset::Unregister(U"font");
	MusicManager::stopMusicGame();
}

void Game::update() {
	gameEndCheck();
	notes->update();
	characterm->update();
	
}

void Game::draw() {
	//背景画像描画
	TextureAsset(U"back").draw();
	notes->draw();
	characterm->draw();
}

bool Game::getClearFlag(void) {
	return isClear;
}
int Game::getTotalDamage(void) {
	return characterm->getTotalDamage();
}
JUDGE::JudgeCount* Game::getJudgeCount(void) {
	return notes->getJudgeCount();
}
void Game::gameEndCheck(void) {
	if (characterm->getCurrentHp() <= 0 && isClear == true) {
		MusicManager::stopMusicGame();
		isClear = false;
		characterm->gameEndEffect();
	}
	if (MusicManager::musicEndCheck() ) {//曲が終わっている　or ゲーム失敗している
		return SceneManager::setNextScene(SceneManager::SCENE_RESULT);//シーン移行
	}
}
