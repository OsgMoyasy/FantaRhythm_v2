#pragma once
#include "FantaRhythm_v2.h"

class SE {
public:
	SE(FilePath& path);
	~SE();
	void play();//SE���Đ�����B�d�����čĐ�����悤�Ƃ������A�ŏ�����Đ����Ȃ���
private:
	Audio* sound;
};

class mapflip {	//�n���ꂽ�}�b�v�`�b�v�摜����`�b�v�摜��؂�o��
public:
	mapflip(const FilePath& path, int xFlipWidth, int yFlipHeight);
	~mapflip();
	bool nextFlip();	//���̈ʒu�̃`�b�v�摜���Z�b�g�B�����Ȃ�(�Ō�)�Ȃ�false��Ԃ�
	TextureRegion getFlip();	//�`�b�v�摜��Ԃ�
private:
	Texture* flip;
	int xFlipWidth;
	int yFlipHeight;
	int xMapWidth;
	int yMapHeight;
	int nowPosX;
	int nowPosY;
};

struct flipMovie :IEffect {//mapflip�𗘗p���ăp���p����������G�t�F�N�g
	flipMovie(const FilePath& path, int xFlipWidth, int yFlipHeight, int drawX, int drawY);
	~flipMovie();
	bool update(double t);
private:
	mapflip* flip;
	int drawX;
	int drawY;
};

struct fractal :IEffect {//�t���N�^���͗l�����o���G�t�F�N�g
	fractal(int size, int x, int y);
	bool update(double t);
private:
	int firstX, firstY;
	int firstSize;
	int x2, y2;

	void drawTriangle(double t, int posX, int posY, int size, int x2, int y2);
};

