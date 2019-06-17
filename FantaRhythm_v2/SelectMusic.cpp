#include "FantaRhythm_v2.h"
#define PI 3.141592


SelectMusic::SelectMusic(void) : cursor(0) {
	TextureAsset::Register(U"back", U"resources/images/back/BackScreen.jpg");
	TextureAsset::Preload(U"back");
	TextureAsset::Register(U"title", U"resources/images/items/title.png");
	TextureAsset::Preload(U"title");

	FontAsset::Register(U"font", 30);
	FontAsset::Preload(U"font");

	//�Ȑ��̎擾
	count = FileSystem::DirectoryContents(U"resources/musics/main", false).count();

	/*�Ȗ��擾*/
	String musicPath;
	for (int i = 0; i < count; i++) {
		musicPath = FileSystem::DirectoryContents(U"resources/musics/main/", false).at(i);
		music.push_back(FileSystem::BaseName(musicPath));
	}

	//�ȃv���r���[�̍Đ�
	playMusic(0);
}

SelectMusic::~SelectMusic(void) {
	TextureAsset::UnregisterAll();
	delete audio;
}

int SelectMusic::update(void) {
	moveCursor();
	rotateMusic();
	if (KeyA.down()) {
		return TITLE;
	}
	return SELECT_MUSIC;
}

void SelectMusic::draw(void) {
	//�w�i�摜�`��
	TextureAsset(U"back").draw();

	/*�Ȗ��̕`��*/
	for (int i = 0; i < 5; i++) {
		//���W�̎w��
		int rotate = 120 + 30 * i + angle;
		int x = 1800 + cos((rotate)* PI / 180.0) * 1000;
		int y = (Window::Height() / 2) - sin((rotate)* PI / 180.0) * 500;
		//�`��
		TextureAsset(U"title").drawAt(x, y);
		FontAsset(U"font")(music[(cursor - 2 + i + count) % count]).drawAt(x, y, Color(0, 0, 0));
	}
}

void SelectMusic::moveCursor(void) {
	if (KeyUp.down()) {
		cursor == 0 ? cursor = count - 1 : cursor--;
		angle = -30;

		//�V�����Ȃ̃Z�b�g&�Đ�
		playMusic(cursor);
	}
	if (KeyDown.down()) {
		cursor == count - 1 ? cursor = 0 : cursor++;
		angle = 30;

		//�V�����Ȃ̃Z�b�g&�Đ�
		delete audio;
		playMusic(cursor);
	}
}

void SelectMusic::rotateMusic(void) {
	if (angle < 0) {
		angle += 3;
	}
	if (angle > 0) {
		angle -= 3;
	}
}

void SelectMusic::playMusic(int musicNum) {
	audio = new Audio(U"resources/musics/previews/" + music[musicNum] + U".wav");
	audio->setLoop(true);
	audio->play();
}