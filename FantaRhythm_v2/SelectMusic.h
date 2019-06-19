#pragma once
class SelectMusic{
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

