#pragma once
#include "FantaRhythm_v2.h"

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