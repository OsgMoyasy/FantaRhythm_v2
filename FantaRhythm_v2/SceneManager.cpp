#include "SceneManager.h"

SceneManager::SceneManager() {
	nowScene = TITLE;
	nextScene = TITLE;
	scene = new Title();
}

void SceneManager::sceneUpdate() {
	//シーンに変更があったとき
	if (nowScene != nextScene) {
		changeScene();
		nowScene = nextScene;
	}
	

	//シーンの呼び出し
	nextScene = scene->update();
	scene->draw();
	
}

void SceneManager::changeScene(){
	//現在のシーンの破棄
	switch (nextScene){
	case TITLE:
		delete scene;
		scene = new Title();
		break;
	case SELECT_MUSIC:
		delete scene;
		scene = new SelectMusic();
		break;
	default:
		break;
	}


}