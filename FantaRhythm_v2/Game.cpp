#include "Game.h"

Game::Game(const String& music, const String& dif) {
	musicpath = music;
	difpath = dif;

	notessubject = new NotesSubject();
	
	notes = new NotesManager(notessubject,difpath);

	characterm = new CharacterSet(musicpath);

	notessubject->addObserver(characterm);//オブザーバーへ登録
	
	isClear = true;//falseになればゲームオーバー


	TextureAsset::Register(U"gameback", U"resources/images/back/"+ FileSystem::FileName(musicpath) +U".jpg");

	TextureAsset::Preload(U"gameback");

	FontAsset::Register(U"gamefont", 30);
	FontAsset::Preload(U"gamefont");

	RankingData::setMusic_name(FileSystem::FileName(musicpath));
	MusicManager::setMusicGame(musicpath);
}
Game::~Game() {
	delete notes;
	delete characterm;
	delete notessubject;
	TextureAsset::Unregister(U"gameback");
	FontAsset::Unregister(U"gamefont");
	MusicManager::setEndMusic();
}

bool Game::isReady() {
	if (TextureAsset::IsReady(U"gameback") && characterm->isReady()) {
		return true;
	}
	return false;
}

void Game::start() {
	MusicManager::playMusic();
}

void Game::update() {
	gameEndCheck();
	notes->update();
	characterm->update();
}

void Game::draw() {
	//背景画像描画
	TextureAsset(U"gameback").draw();
	characterm->draw();
	notes->draw();
	
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
		MusicManager::setEndMusic();
		isClear = false;
		characterm->gameEndEffect();
	}
	if (MusicManager::musicEndCheck() ) {//曲が終わっている　or ゲーム失敗している
		return SceneManager::setNextScene(SceneManager::SCENE_RESULT);//シーン移行
	}
}
