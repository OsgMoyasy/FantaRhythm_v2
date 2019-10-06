#include "Game.h"

Game::Game(const String& music, const String& dif) {
	musicpath = music;
	difpath = dif;

	notessubject = new NotesSubject();
	
	notes = new NotesManager(notessubject,difpath);
	
	int save[4] = { 1,4,5,6 };//�e�X�g�p�Z�[�u�f�[�^
	characterm = new CharacterSet(save, musicpath);

	notessubject->addObserver(characterm);//�I�u�U�[�o�[�֓o�^
	
	isClear = true;//false�ɂȂ�΃Q�[���I�[�o�[
	TextureAsset::Register(U"gameback", U"resources/images/back/first.jpg");
	TextureAsset::Preload(U"gameback");

	FontAsset::Register(U"gamefont", 30);
	FontAsset::Preload(U"gamefont");
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

void Game::start() {
	MusicManager::playMusic();
}

void Game::update() {
	gameEndCheck();
	notes->update();
	characterm->update();
}

void Game::draw() {
	//�w�i�摜�`��
	TextureAsset(U"gameback").draw();
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
		MusicManager::setEndMusic();
		isClear = false;
		characterm->gameEndEffect();
	}
	if (MusicManager::musicEndCheck() ) {//�Ȃ��I����Ă���@or �Q�[�����s���Ă���
		return SceneManager::setNextScene(SceneManager::SCENE_RESULT);//�V�[���ڍs
	}
}
