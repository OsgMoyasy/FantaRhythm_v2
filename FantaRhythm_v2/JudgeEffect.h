#pragma once
#include "Judge.h"
#include <Siv3D.hpp>

class JudgeEffect {
public:
	JudgeEffect(FilePath path);	//�W���b�W���G�t�F�N�g�̃t�@�C���p�X
	~JudgeEffect();
	void setEffect(JUDGE::TYPE type);
	void update();
	void draw();
private:
	Texture texture[JUDGE::TYPE_SIZE];
	double alpha;
	const double drawSecond = 0.5;//�`�掞��
	const double drawFixedSecond = 0.3;//�`�掞�̓��ߗ���1�ɂȂ�܂ł̎���
	Stopwatch stopwatch;
	JUDGE::TYPE type;
	

};