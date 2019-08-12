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

struct SE {
	SE(FilePath& path);
	~SE();
	void play();
private:
	Audio* sound;
};

struct mapchip {	//�n���ꂽ�}�b�v�`�b�v�摜����`�b�v�摜��؂�o��
	mapchip(const FilePath& path, int yChipHeight, int xChipWidth);
	~mapchip();
	TextureRegion chipFromMap();	//�Ă΂��x�Ɏ��̈ʒu�̃`�b�v�摜�̃|�C���^��Ԃ�
private:
	Texture* map;
	int yChipHeight;
	int xChipWidth;
	int yMapHeight;
	int xMapWidth;
};