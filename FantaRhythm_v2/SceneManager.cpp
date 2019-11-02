#include "SceneManager.h"


Scene* SceneManager::scene;
Scene* SceneManager::tmpscene;
SceneManager::SCENE SceneManager::NOWSCENE;
SceneManager::SCENE SceneManager::NEXTSCENE;
LoadEffect* SceneManager::loadeffect;
bool SceneManager::isLoadFinished;


void SceneManager::initialize() {
	scene = new Title();
	((Title*)scene)->start();
	loadeffect = new LoadEffect();
	isLoadFinished = true;
}
void SceneManager::finalize() {
	delete scene;
}
void SceneManager::updateScene() {
	if (NOWSCENE != NEXTSCENE) {
		isLoadFinished = false;
		loadeffect->setTimer(1);
		NOWSCENE = NEXTSCENE;
	}
	if (!isLoadFinished) {
		if (loadeffect->isLoadStanby()) {
			changeScene();
		}
		if (!loadeffect->isUpdate(scene->isReady())) {
			scene->start();
			isLoadFinished = true;
		}
		
	}
	scene->update();
}
void SceneManager::drawScene() {
	scene->draw();
	loadeffect->draw();
}

void SceneManager::setNextScene(SCENE next) {
	NEXTSCENE = next;
}

void SceneManager::changeScene() {
	
	switch (NEXTSCENE) {//nextsceneがNONE以外の時シーン移行する
	case SCENE_TITLE:
		delete scene;
		loadeffect->setTimer(1);
		scene = new Title();
		break;
	case SCENE_QRREAD:
		delete scene;
		loadeffect->setTimer(1);
		scene = new QrRead();
		break;
	case SCENE_SELECTMUSIC:
		delete scene;
		loadeffect->setTimer(1);
		scene = new SelectMusic();
		break;
	case SCENE_GAME: {
		loadeffect->setTimer(2);
		//曲のパスと難易度選択のパスを退避

		String musicpath = ((SelectMusic*)scene)->getMusicPath();
		String filepath = ((SelectMusic*)scene)->getDifPath();
		delete scene;
		scene = new Game(musicpath, filepath);
		break;
	}
	case SCENE_RESULT: {
		loadeffect->setTimer(1);
		//判定のカウント数と敵への総ダメージ量を退避
		int totalDamage = ((Game*)scene)->getTotalDamage();
		bool isClear = ((Game*)scene)->getClearFlag();
		JUDGE::JudgeCount jc = *((Game*)scene)->getJudgeCount();
		delete scene;
		scene = new Result(jc, totalDamage, isClear);
		break;
	}
	default:
		break;
	}

}
