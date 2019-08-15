#pragma once
#include "Scene.h"
#include "SceneManager.h"
#include "NotesManager.h"
#include "Observer.h"
#include "Enemy.h"
#include "SceneManager.h"
#include "MusicManager.h"

namespace JUDGE {
	union _JudgeCount;
	typedef _JudgeCount JudgeCount;
}

class Game : public Scene {
private:
	String musicpath, difpath;
	class NotesManager *notes;
	class CharacterManager* character;
	class NotesSubject* subject;
	class Enemy* enemy;

	bool clearflag;

public:
	

	Game(const String& music, const String& dif);
	~Game();

	void update(void);
	void draw(void);
	bool getClearFlag(void);
	int getTotalDamage(void);
	JUDGE::JudgeCount* getJudgeCount(void);
	void gameEndCheck(void);

};