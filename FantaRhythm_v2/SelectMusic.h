#pragma once
#include "Scene.h"

class SelectMusic:public Scene{
public:
	SelectMusic(void);
	~SelectMusic(void);	
	int update(void);
	void draw(void);
private:
	std::vector<String> music;
	int musicCount;
	int cursor;
	int rotation;
	Audio *audio;
	void moveCursor(void);
	void rotateMusic(void);
	void playMusic(int musicNum);
};

