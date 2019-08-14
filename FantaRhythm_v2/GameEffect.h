#pragma once
#include "FantaRhythm_v2.h"

/*
namespace SE {
	enum NAME {
		HIT
	};
	enum TYPE {//����SE�������Đ����ꂽ���̏���
		DUL,	//�d��
		REP,	//�ŏ�����Đ�
		IGN		//����
	};
}
namespace VE {
	enum NAME {
		HIT
	};
}
namespace ME {
	enum NAME {
		HIT
	};
}

class GameEffect {
public:
	static void playSE(SE::NAME name, SE::TYPE type);	//���ʉ�
	static void playVE(VE::NAME name);	//���o���o
	static void playME(ME::NAME name);	//���ʉ��Ǝ��o���o�̃Z�b�g
};
*/

class SE {
	Audio* sound;
public:
	SE(FilePath& path);
	~SE();
	void play();//SE���Đ�����B�d�����čĐ�����悤�Ƃ������A�ŏ�����Đ����Ȃ���
};

class mapflip {	//�n���ꂽ�}�b�v�`�b�v�摜����`�b�v�摜��؂�o��
	Texture* flip;
	int xFlipWidth;
	int yFlipHeight;
	int xMapWidth;
	int yMapHeight;
	int nowPosX;
	int nowPosY;
public:
	mapflip(const FilePath& path, int xFlipWidth, int yFlipHeight);
	~mapflip();
	bool mapflip::nextFlip();	//���̈ʒu�̃`�b�v�摜���Z�b�g�B�����Ȃ�(�Ō�)�Ȃ�false��Ԃ�
	TextureRegion getFlip();	//�`�b�v�摜��Ԃ�
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
	int firstX, firstY;
	int firstSize;
	int x2, y2;
	fractal(int size, int x, int y);
	bool update(double t);
private:
	void drawTriangle(double t, int posX, int posY, int size, int x2, int y2);
};

