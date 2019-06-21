#pragma once
#include "SelectScene.h"
#include <vector>

class SelectMusic:public SelectScene{
public:
	SelectMusic(std::vector<String>& elementName, int& elementCount);
	~SelectMusic(void);
	void getElementName(std::vector<String>& elementName, int elementCount);
	void update(int& rotation, int elementCount);
	int getCursor(void);
private:
	int cursor;
	String getMusicName(int musicNum);
	void moveCursor(int& rotation, int elementCount);
	void rotateMusic(int& rotation);
	void playMusic(const int& rotation);
};