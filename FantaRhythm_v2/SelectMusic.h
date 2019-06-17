#pragma once
class SelectMusic{
public:
	SelectMusic(void);
	~SelectMusic(void);	
	int update(void);
	void draw(void);
private:
	std::vector<String> music;
	int count;
	int cursor;
	int angle;
	Audio *audio;
	void moveCursor(void);
	void rotateMusic(void);
	void playMusic(int musicNum);
};

