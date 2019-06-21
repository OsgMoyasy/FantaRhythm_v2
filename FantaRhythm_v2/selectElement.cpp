#include "SelectElement.h"
#include "SelectMusic.h"

#define PI 3.141592


SelectElement::SelectElement() : switcher(MUSIC), rotation(0){
	TextureAsset::Register(U"back", U"resources/images/back/BackScreen.jpg");
	TextureAsset::Preload(U"back");
	TextureAsset::Register(U"title", U"resources/images/items/title.png");
	TextureAsset::Preload(U"title");

	FontAsset::Register(U"font", 30);
	FontAsset::Preload(U"font");

	scene.push(new SelectMusic(elementName, elementCount));
}


SelectElement::~SelectElement(){
	TextureAsset::UnregisterAll();
	FontAsset::Unregister(U"font");
}


int SelectElement::update(void) {

	//��]�r���ɃV�[���J�ڂ����Ȃ�
	if (rotation == 0) {
		//����{�^��
		if (KeyA.down()) {
			if (switcher == DIFFICULTY) {
				//���̃V�[���ɐ؂�ւ�(���V�[�����Ȃ����ߑ����TITLE�ɑJ��)
				return TITLE;
			}
			else {
				//��Փx�I���ɐ؂�ւ�
				//��Փx�I�����Ȃ����ߑ���ɋȑI���Ɉړ�
				//�ړ������Ƃ��Ȃ��ς��Ȃ��͎̂d�l�Ȃ̂Ŗ��Ȃ�
				switcher = DIFFICULTY;
				scene.push(new SelectMusic(elementName, elementCount));
			}
		}

		//�L�����Z���{�^��
		if (KeyS.down()) {
			if (switcher == MUSIC) {
				//���̃V�[���ɐ؂�ւ�
				return TITLE;
			}
			else {
				//�ȑI���ɖ߂�
				switcher = MUSIC;
				delete scene.top();
				scene.pop();
			}
		}
	}

	scene.top()->update(rotation, elementCount);

	return SELECT_ELEMENT;
}


void SelectElement::draw() {
	//�w�i�摜�`��
	TextureAsset(U"back").draw();

	/*�Ȗ��̕`��*/
	for (int i = 0; i < 5; i++) {
		//���W�̎w��
		int angle = 120 + 30 * i + rotation;
		int x = 1800 + cos((angle)* PI / 180.0) * 1000;
		int y = (Window::Height() / 2) - sin((angle)* PI / 180.0) * 500;
		//�`��
		TextureAsset(U"title").drawAt(x, y);
		FontAsset(U"font")(elementName[(scene.top()->getCursor() - 2 + i + elementCount) % elementCount]).drawAt(x, y, Color(0, 0, 0));
	}
}