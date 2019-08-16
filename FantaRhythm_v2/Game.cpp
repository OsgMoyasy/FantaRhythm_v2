#include "Game.h"

Game::Game(const String& music, const String& dif) {
	musicpath = music;
	difpath = dif;

	subject = new NotesSubject();
	
	notes = new NotesManager(subject,difpath);
	enemy = new Enemy(music);
	
	clearflag = false;
	
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
	enemy->update();
}
void Game::draw() {
	//”wŒi‰æ‘œ•`‰æ
	TextureAsset(U"back").draw();
	notes->draw();
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
	//clearflag = HP‚ª0‚¾‚Á‚½‚çfalse
	if (MusicManager::musicEndCheck() || clearflag == false) {//‹È‚ªI‚í‚Á‚Ä‚¢‚é@or ƒQ[ƒ€¸”s‚µ‚Ä‚¢‚é
		return SceneManager::setNextScene(SceneManager::SCENE_RESULT);//ƒV[ƒ“ˆÚs
	}
}
