#pragma once
#include "FantaRhythm_v2.h"
# include <Siv3D.hpp> // OpenSiv3D v0.3.2
#include "Scene.h"
#include "Title.h"
#include "SelectMusic.h"
#include "Game.h"
#include "Result.h"
#include "NotesManager.h"
#include "Judge.h"

class SceneManager {
public:
	enum SCENE {
		SCENE_TITLE,
		SCENE_SELECTMUSIC,
		SCENE_GAME,
		SCENE_RESULT,
		SCENE_NONE
	};
	SceneManager() = delete;
	static void initialize();
	static void finalize();

	static void updateScene();
	static void drawScene();
	static void setNextScene(SCENE next);//次フレームにてシーンが移行する
private:
	static Scene *scene;
	static SCENE nowscene;
	static SCENE nextscene;//シーン移行時にはNONE以外が入る
	static void changeScene();//シーン移行
};
