#pragma once
#include "Scene.h"
#include "SceneManager.h"
#include "NotesManager.h"
#include "Observer.h"
#include "CharacterSet.h"
#include "Enemy.h"
#include "MusicManager.h"
#include "Judge.h"

class Game : public Scene {
private:
	String musicpath, difpath;
	class NotesManager *notes;
	class CharacterManager* character;
	class NotesSubject* subject;

public:
	

	Game(const String& music, const String& dif);
	~Game();

	void update(void);
	void draw(void);

};