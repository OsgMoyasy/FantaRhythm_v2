#include "SceneManager.h"

SceneManager::SceneManager() {
	nowScene = TITLE;
	nextScene = TITLE;
	title = new Title();
}

void SceneManager::sceneUpdate() {
	//シーンに変更があったとき
	if (nowScene != nextScene) {
		changeScene();
		nowScene = nextScene;
	}

	//シーンの呼び出し
	switch (nowScene){
	case TITLE:
		nextScene = title->update();
		title->draw();
		break;

	case SELECT_MUSIC:
		nextScene = selectMusic->update();
		selectMusic->draw();
		break;

	default:

		break;
	}
}

void SceneManager::changeScene(){
	//現在のシーンの破棄
	switch (nowScene){
	case TITLE:
		delete title;
		break;
	case SELECT_MUSIC:
		delete selectMusic;
		break;
	default:
		break;
	}

	//次のシーンの初期化
	switch (nextScene){
	case TITLE:
		title = new Title();
		break;

	case SELECT_MUSIC:
		selectMusic = new SelectMusic();
		break;

	default:
		break;
	}
}