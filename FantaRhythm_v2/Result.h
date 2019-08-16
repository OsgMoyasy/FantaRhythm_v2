#pragma once
#include "NotesManager.h"
#include "FantaRhythm_v2.h"
#include "Scene.h"



namespace JUDGE {
	union _JudgeCount;
	typedef _JudgeCount JudgeCount;
}

class Result : public Scene {
public:
	Result(JUDGE::JudgeCount* judgecnt, int totaldmg, bool cflag);
	~Result(void);
	void update(void);
	void draw(void);

private:
	int totaldamage;
	JUDGE::JudgeCount* judgecount;
	bool clearflag;

	int framecnt;
	static const int alphatime = 3 * 60;//�A���t�@�l�����ɖ߂�܂ł̎��ԁ~�t���[����
	double alpha;
	double alphafont;

	void(Result::* stateUpdate)(void);//���s����v�Z�����ւ̊֐��|�C���^
	void(Result::* stateDraw)(void);//���s����`�揈���ւ̊֐��|�C���^

	//�Q�[���N���A�p
	int score;


	void calcScore(void);//�X�R�A�v�Z
	void successUpdate(void);
	void successDraw(void);
	//�Q�[���I�[�o�[�p
	void failedUpdate(void);
	void failedDraw(void);
};
