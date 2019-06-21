#include "FantaRhythm_v2.h"
#include "SelectMusic.h"

SelectMusic::SelectMusic(std::vector<String>& elementName, int& elementCount) : cursor(0){
	elementCount = FileSystem::DirectoryContents(U"resources/musics/main/", false).count();
	getElementName(elementName, elementCount);

	AudioAsset::Register(U"music", U"resources/musics/main/" + getMusicName(0) + U"/preview.wav");
	AudioAsset(U"music").play();
}

SelectMusic::~SelectMusic(void) {
	return;
}

void SelectMusic::update(int& rotation, int elementCount) {
	playMusic(rotation);
	moveCursor(rotation, elementCount);
	rotateMusic(rotation);
}

void SelectMusic::getElementName(std::vector<String>& elementName, int elementCount) {
	//elementNameの要素を全削除
	std::vector<String>().swap(elementName);

	String musicPath;
	for (int i = 0; i < elementCount; i++) {
		musicPath = FileSystem::DirectoryContents(U"resources/musics/main/", false).at(i);
		elementName.push_back(FileSystem::BaseName(musicPath));
	}
}

String SelectMusic::getMusicName(int musicNum) {
	String musicPath = FileSystem::DirectoryContents(U"resources/musics/main/", false).at(musicNum);
	return FileSystem::BaseName(musicPath);
}

void SelectMusic::moveCursor(int& rotation, int elementCount) {
	if (KeyUp.down()) {
		cursor == 0 ? cursor = elementCount - 1 : cursor--;
		//曲名を回転させるため角度を30度マイナス
		rotation = -30;
	}
	if (KeyDown.down()) {
		cursor == elementCount - 1 ? cursor = 0 : cursor++;
		//曲名を回転させるため角度を30度プラス
		rotation = 30;
	}
}

void SelectMusic::rotateMusic(int& rotation) {
	if (rotation < 0) {
		//既定の位置にくるまで1フレームおきに角度をプラス
		rotation += 3;
	}
	if (rotation > 0) {
		//既定の位置にくるまで1フレームおきに角度をマイナス
		rotation -= 3;
	}
}

int SelectMusic::getCursor(void){
	return cursor;
}

void SelectMusic::playMusic(const int& rotation) {
	//音楽の再生
//上下ボタン連打した時にフレームレートが無茶苦茶下がるのの対策
	if (rotation == 3 || rotation == -3) {
		AudioAsset::Unregister(U"music");
		AudioAsset::Register(U"music", U"resources/musics/main/" + getMusicName(cursor) + U"/preview.wav");
		AudioAsset(U"music").play();
	}
};