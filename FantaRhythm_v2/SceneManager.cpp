#include "SceneManager.h"

SceneManager::SceneManager() {
	nowScene = TITLE;
	nextScene = TITLE;
	scene = new Title();
}

void SceneManager::sceneUpdate() {
	//�V�[���ɕύX���������Ƃ�
	if (nowScene != nextScene) {
		changeScene();
		nowScene = nextScene;
	}
	

	//�V�[���̌Ăяo��
	nextScene = scene->update();
	scene->draw();
	
}

void SceneManager::changeScene(){
	//���݂̃V�[���̔j��
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