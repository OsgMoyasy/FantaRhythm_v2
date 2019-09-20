#include "Game.h"

Game::Game(const String& music, const String& dif) {
	musicpath = music;
	difpath = dif;

	notessubject = new NotesSubject();
	
	notes = new NotesManager(notessubject,difpath);
	
	
	int save[4] = { 0,0,1,1 };//�e�X�g�p�Z�[�u�f�[�^
	characterm = new CharacterSet(save,musicpath);

	notessubject->addObserver(characterm);//�I�u�U�[�o�[�֓o�^
	
	isClear = true;//false�ɂȂ�΃Q�[���I�[�o�[
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
	//�w�i�摜�`��
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
	if (MusicManager::musicEndCheck() ) {//�Ȃ��I����Ă���@or �Q�[�����s���Ă���
		return SceneManager::setNextScene(SceneManager::SCENE_RESULT);//�V�[���ڍs
	}
}
