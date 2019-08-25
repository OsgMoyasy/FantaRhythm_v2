#pragma once
#include "FantaRhythm_v2.h"
#include "Scene.h"
#include "Judge.h"
#include <Siv3D.hpp>



class Result : public Scene {
public:
	Result(JUDGE::JudgeCount judgecnt, int totaldmg, bool cflag);
	~Result(void);
	void update(void);
	void draw(void);

private:
	int totaldamage;
	JUDGE::JudgeCount judgecount;
	bool clearflag;

	int framecnt;
	static const int alphatime = 3 * 60;//�A���t�@�l�����ɖ߂�܂ł̎��ԁ~�t���[����
	double alpha;
	double alphafont;

	void(Result::* stateUpdate)(void);//���s����v�Z�����ւ̊֐��|�C���^
	void(Result::* stateDraw)(void);//���s����`�揈���ւ̊֐��|�C���^

	//�Q�[���N���A�p
	String scoreStr;//�X�R�A�𐔒l�ϊ���������
	String scoreDraw;//���ۂɕ`�悷��X�R�A

	void scoreEffect(void);



	int calcScore(JUDGE::JudgeCount &jc);//�X�R�A�v�Z
	void successUpdate(void);
	void successDraw(void);
	//�Q�[���I�[�o�[�p
	void failedUpdate(void);
	void failedDraw(void);
};
