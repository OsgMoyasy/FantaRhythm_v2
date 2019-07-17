#include "Game.h"
#include "FantaRhythm_v2.h"
#include "MusicManager.h"

Game::Game(const String& music, const String& dif) {
	musicpath = music;
	difpath = dif;

	notes = new NotesManager(difpath);
	
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


}
void Game::draw() {
	//”wŒi‰æ‘œ•`‰æ
	TextureAsset(U"back").draw();
	notes->draw();
}