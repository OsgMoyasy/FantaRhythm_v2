#include "Game.h"

Game::Game(const String& music, const String& dif) {
	musicpath = music;
	difpath = dif;

	subject = new NotesSubject();
	
	notes = new NotesManager(subject,difpath);
	enemy = new Enemy(music);
	
	
	
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
}

void Game::update() {
	notes->update();
	enemy->update();
}
void Game::draw() {
	//”wŒi‰æ‘œ•`‰æ
	TextureAsset(U"back").draw();
	notes->draw();
	enemy->draw();
}