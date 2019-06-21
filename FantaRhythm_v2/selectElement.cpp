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

	//回転途中にシーン遷移させない
	if (rotation == 0) {
		//決定ボタン
		if (KeyA.down()) {
			if (switcher == DIFFICULTY) {
				//次のシーンに切り替え(次シーンがないため代わりにTITLEに遷移)
				return TITLE;
			}
			else {
				//難易度選択に切り替え
				//難易度選択がないため代わりに曲選択に移動
				//移動したとき曲が変わらないのは仕様なので問題なし
				switcher = DIFFICULTY;
				scene.push(new SelectMusic(elementName, elementCount));
			}
		}

		//キャンセルボタン
		if (KeyS.down()) {
			if (switcher == MUSIC) {
				//次のシーンに切り替え
				return TITLE;
			}
			else {
				//曲選択に戻る
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
	//背景画像描画
	TextureAsset(U"back").draw();

	/*曲名の描画*/
	for (int i = 0; i < 5; i++) {
		//座標の指定
		int angle = 120 + 30 * i + rotation;
		int x = 1800 + cos((angle)* PI / 180.0) * 1000;
		int y = (Window::Height() / 2) - sin((angle)* PI / 180.0) * 500;
		//描画
		TextureAsset(U"title").drawAt(x, y);
		FontAsset(U"font")(elementName[(scene.top()->getCursor() - 2 + i + elementCount) % elementCount]).drawAt(x, y, Color(0, 0, 0));
	}
}