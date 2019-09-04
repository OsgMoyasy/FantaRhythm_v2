#include "SceneManager.h"

Scene* SceneManager::scene;
SceneManager::SCENE SceneManager::nowscene;
SceneManager::SCENE SceneManager::nextscene;


void SceneManager::initialize() {
	scene = new Title();
}
void SceneManager::finalize() {
	delete scene;
}
void SceneManager::updateScene() {
	if (nowscene != nextscene) {
		changeScene();
	}
	scene->update();
}
void SceneManager::drawScene() {
	scene->draw();
}

void SceneManager::setNextScene(SCENE next) {
	nextscene = next;
}

void SceneManager::changeScene() {
	switch (nextscene) {//nextscene��NONE�ȊO�̎��V�[���ڍs����
	case SCENE_TITLE:
		delete scene;
		scene = new Title();
		break;
	case SCENE_SELECTMUSIC:
		delete scene;
		scene = new SelectMusic();
		break;
	case SCENE_GAME:
		if (nowscene == SCENE_SELECTMUSIC) {
			//�Ȃ̃p�X�Ɠ�Փx�I���̃p�X��ޔ�
			String musicpath = ((SelectMusic*)scene)->getMusicPath();
			String filepath = ((SelectMusic*)scene)->getDifPath();
			delete scene;
			scene = new Game(musicpath,filepath);
		}
		else {
			nextscene = SCENE_NONE;
		}
		break;
	case SCENE_RESULT:
		if (nowscene == SCENE_GAME) {
			//����̃J�E���g���ƓG�ւ̑��_���[�W�ʂ�ޔ�
			int totaldmg = ((Game*)scene)->getTotalDamage();
			bool clearflag = ((Game*)scene)->getClearFlag();
			JUDGE::JudgeCount jc = *((Game*)scene)->getJudgeCount();
			delete scene;
			scene = new Result(jc, totaldmg, clearflag);
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
