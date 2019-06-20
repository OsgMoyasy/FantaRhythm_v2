#pragma once
#include "FantaRhythm_v2.h"
#include "Scene.h"

class SceneManager {
public:
	SceneManager();
	void sceneUpdate();
private:
	int nextScene;
	int nowScene;
	void changeScene();
	Scene *scene;
};
