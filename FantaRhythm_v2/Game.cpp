#include "Game.h"

Game::Game(const String& music, const String& dif) {
	musicpath = music;
	difpath = dif;

	subject = new NotesSubject();
	
	notes = new NotesManager(subject,difpath);
	enemy = new Enemy(music);
	
	int tmp[4] = { 0,0,1,1 };//テスト用セーブデータ
	characterm = new CharacterSet(tmp);

	subject->addObserver(characterm);//オブザーバーへ登録
	
	clearflag = true;//falseになればゲームオーバー
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
	enemy->update();
}

void Game::draw() {
	//背景画像描画
	TextureAsset(U"back").draw();
	notes->draw();
	characterm->draw();
}
	enemy->draw();
}

bool Game::getClearFlag(void) {
	return clearflag;
}
int Game::getTotalDamage(void) {
	return enemy->getTotalDamage();
}
JUDGE::JudgeCount* Game::getJudgeCount(void) {
	return notes->getJudgeCount();
}
void Game::gameEndCheck(void) {
	if (MusicManager::musicEndCheck() || clearflag == false) {//曲が終わっている　or ゲーム失敗している
		return SceneManager::setNextScene(SceneManager::SCENE_RESULT);//シーン移行
	}
}

