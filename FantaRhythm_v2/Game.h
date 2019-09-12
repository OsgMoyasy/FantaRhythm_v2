#pragma once
#include "Scene.h"
#include "SceneManager.h"
#include "NotesManager.h"
#include "Observer.h"
#include "CharacterSet.h"
#include "FantaRhythm_v2.h"
#include "SceneManager.h"
#include "MusicManager.h"


class Game : public Scene {
private:
	String musicpath, difpath;
	class NotesManager *notes;
	class CharacterSet* characterm;
	class NotesSubject* notessubject;
	
	bool clearFlag;

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