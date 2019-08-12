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
	void play();
};

class mapflip {	//�n���ꂽ�}�b�v�`�b�v�摜����`�b�v�摜��؂�o��
	Texture* map;
	int yFlipHeight;
	int xFlipWidth;
	int yMapHeight;
	int xMapWidth;
	int nowPosX;
	int nowPosY;
public:
	mapflip(const FilePath& path, int yFlipHeight, int xFlipWidth);
	~mapflip();
	bool mapflip::nextFlip();
	TextureRegion* flipFromMap();	//�Ă΂��x�Ɏ��̈ʒu�̃`�b�v�摜�̃|�C���^��Ԃ�
	inline TextureRegion* operator()() { return flipFromMap(); }
};

class flipMovie :IEffect {
	mapflip* flip;
	int drawX;
	int drawY;
public:
	flipMovie(const FilePath& path, int yFlipHeight, int xFlipWidth, int drawX, int drawY);
	~flipMovie();
	bool update(double t);
};