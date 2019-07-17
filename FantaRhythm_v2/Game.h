#pragma once
#include "Scene.h"
#include "SceneManager.h"
#include "NotesManager.h"


class Game : public Scene {
private:
	String musicpath, difpath;
	class NotesManager *notes;
public:
	Game(const String& music, const String& dif);
	~Game();
	void update(void);
	void draw(void);

};