#include "SceneManager.h"


Scene* SceneManager::scene;
SceneManager::SCENE SceneManager::nowscene;
SceneManager::SCENE SceneManager::nextscene;
LoadEffect* SceneManager::loadeffect;
bool SceneManager::isLoadFinished;


void SceneManager::initialize() {
	scene = new Title();
	loadeffect = new LoadEffect();
	isLoadFinished = true;
}
void SceneManager::finalize() {
	delete scene;
}
void SceneManager::updateScene() {
	if (nowscene != nextscene) {
		changeScene();
		isLoadFinished = false;
	}
	if (!isLoadFinished) {
		if (!loadeffect->isUpdate()) {
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
	nextscene = next;
}

void SceneManager::changeScene() {
	switch (nextscene) {//nextscene��NONE�ȊO�̎��V�[���ڍs����
	case SCENE_TITLE:
		delete scene;
		scene = new Title();
		loadeffect->setTimer(2);
		break;
	case SCENE_SELECTMUSIC:
		delete scene;
		scene = new SelectMusic();
		loadeffect->setTimer(2);
		break;
	case SCENE_GAME:
		if (nowscene == SCENE_SELECTMUSIC) {
			//�Ȃ̃p�X�Ɠ�Փx�I���̃p�X��ޔ�
			String musicpath = ((SelectMusic*)scene)->getMusicPath();
			String filepath = ((SelectMusic*)scene)->getDifPath();
			delete scene;
			scene = new Game(musicpath,filepath);
			loadeffect->setTimer(2);
		}
		else {
			nextscene = SCENE_NONE;
		}
		break;
	case SCENE_RESULT:
		if (nowscene == SCENE_GAME) {
			//����̃J�E���g���ƓG�ւ̑��_���[�W�ʂ�ޔ�
			int totalDamage = ((Game*)scene)->getTotalDamage();
			bool isClear = ((Game*)scene)->getClearFlag();
			JUDGE::JudgeCount jc = *((Game*)scene)->getJudgeCount();
			delete scene;
			scene = new Result(jc, totalDamage, isClear);
			loadeffect->setTimer(2);
		}
		else {
			nextscene = SCENE_NONE;
		}
		break;
	default:
		break;
	}
	nowscene = nextscene;
}
