#include "Party.h"


Party::Party() {
	TextureAsset::Register(U"partyback", U"resources/images/back/BackScreen.jpg", AssetParameter::LoadAsync());
	TextureAsset::Register(U"partymsg", U"resources/images/items/massage.png", AssetParameter::LoadAsync());
	FontAsset::Register(U"partyfont", 50);
	FontAsset::Preload(U"partyfont");

	msg = U"左６つのボタンでキャラを４体選択してください";
	msgX = Window::Width() / 2;
	msgY = 100;
}
Party::~Party() {
	TextureAsset::Unregister(U"partyback");
	TextureAsset::Unregister(U"partymsg");
	FontAsset::Unregister(U"partyfont");
}
bool Party::isReady(void) {
	if (TextureAsset::IsReady(U"partyback") &&
		TextureAsset::IsReady(U"partymsg")) {
		return true;
	}
	return false;
}
void Party::start(void) {

}
void Party::update(void) {
	if (char_vec.size() == CHARACTER_SIZE) {
		SceneManager::setNextScene(SceneManager::SCENE_SELECTMUSIC);
	}
	else if(MyKey::getReturnKey()){
		if (char_vec.size() > 0) {
			char_vec.pop_back();//１つ要素削除
		}
		else {
			SceneManager::setNextScene(SceneManager::SCENE_TITLE);
		}
	}

	//キャラ選択
	int down[4];
	int setIndex = -1;
	MyKey::getGameDownKey(down);
	for (int i = 0; i < 3; i++) {
		for (int j = 1; j <= 2; j++) {
			if (down[i] == j) {
				setIndex = j;
				break;
			}
		}
	}
	if (setIndex != -1) {
		if (canCharSet(setIndex)) {//キャラ追加
			char_vec.push_back(setIndex);
		}
	}
	
	
}
void Party::draw(void) {
	TextureAsset(U"qrreadback").draw();
	TextureAsset(U"qrreadmsg").drawAt(msgX, msgY);
	FontAsset(U"qrreadfont")(msg).drawAt(msgX, msgY - 10, ColorF(0, 0, 0));
}

bool Party::canCharSet(int character_id) {
	if (char_vec.size() == CHARACTER_SIZE) { return false; };
	for (int i = 0; i < char_vec.size(); i++) {
		if (char_vec.at(i) == character_id) {
			return false;
		}
	}
	return true;
}