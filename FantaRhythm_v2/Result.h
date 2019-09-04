#pragma once
#include "FantaRhythm_v2.h"
#include "Scene.h"
#include "Judge.h"
#include <Siv3D.hpp>


class Result : public Scene {
public:
	Result(JUDGE::JudgeCount judgecnt, int totaldmg, bool clearflag);
	~Result(void);
	void update(void);
	void draw(void);

private:
	int totaldmg;
	JUDGE::JudgeCount judgecnt;
	bool clearflag;

	int framecnt;
	static const int alphatime = 3 * 60;//�A���t�@�l�����ɖ߂�܂ł̎��ԁ~�t���[����
	double alphaBack;
	double alphaFont;

	void(Result::* stateUpdate)(void);//���s����v�Z�����ւ̊֐��|�C���^
	void(Result::* stateDraw)(void);//���s����`�揈���ւ̊֐��|�C���^

	void changeFontAlpha(void);//�T�C�N���̎����Ńt�H���g�̓����x�ύX

	//�Q�[���N���A�p

	String scoreStr;//�X�R�A�𕶎���ϊ�
	String scoreDraw;//���ۂɕ`�悷��X�R�A

	void scoreEffect(void); //�X�R�A��0~9�Ə��Ԃɕς���ĉ��ʂ̌�����m�肳����悤�ɂ���
	int calcScore(JUDGE::JudgeCount &judgecnt);//�ŏI�I�ȃX�R�A�v�Z
	void successUpdate(void);
	void successDraw(void);

	//�Q�[���I�[�o�[�p
	void failedUpdate(void);
	void failedDraw(void);
};
