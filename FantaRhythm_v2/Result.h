#pragma once
#include "Scene.h"
#include "Judge.h"
#include "GameEffect.h"
#include <Siv3D.hpp>


class Result : public Scene {
public:
	Result(JUDGE::JudgeCount judgeCnt, int totalDamage, bool isClear);
	~Result(void);
	void update(void);
	void draw(void);

private:
	Stopwatch stopwatch;
	class SE* se;
	class ImageNumber* judgeImNum;
	class NumWithEffect* scoreNumEffect;
	class NumWithEffect* damageNumEffect;
	
	int score;
	int totalDamage;
	JUDGE::JudgeCount judgeCnt;
	bool isClear;

	int framecnt;
	static const int alphatime = 3 * 60;//�A���t�@�l�����ɖ߂�܂ł̎��ԁ~�t���[����
	double alphaBack;
	double alphaFont;

	void(Result::* stateUpdate)(void);//���s����v�Z�����ւ̊֐��|�C���^
	void(Result::* stateDraw)(void);//���s����`�揈���ւ̊֐��|�C���^

	void changeFontAlpha(void);//�T�C�N���̎����Ńt�H���g�̓����x�ύX

	//�Q�[���N���A�p

	String scoreStr;//�X�R�A�𕶎���ϊ�
	String damageStr;//�_���[�W�𕶎���ϊ�

	void successUpdate(void);
	void successDraw(void);

	int calcScore(JUDGE::JudgeCount& judgeCnt);//�X�R�A�v�Z
	void imNumberInit(void);//�摜�̐����Ɋւ��鏉�����@���g�p���镶����̏���������ɌĂяo������
	bool judgeUpdate();//���萔�����Ԃɉ������ɕ`�悵�Ă������߂̊֐�
	

	//�Q�[���I�[�o�[�p
	void failedUpdate(void);
	void failedDraw(void);
};


class ImageNumber {
private:
	Texture imNumber[10];
	int imw, imh;

	typedef struct _NumPoint {
		int x, y;
		int num;
	}NumPoint;

	std::vector<NumPoint> numberp;//�ǉ����ꂽ������S�ĕێ�


public:
	ImageNumber(FilePath path, int w, int h);
	~ImageNumber();
	void addOne(int num, int x, int y);//�������P�����ǉ�����
	void ImageNumber::addMulti(int num, int x, int y);//�������ǉ�����
	void draw();

};

class NumWithEffect {
private:
	String numStr;
	class FlipEffect* numEffect;
	int fixedAtTime;
	int currentWord;//���݊m�肳���悤�Ƃ��Ă��鐔��
	double prevtime;
	ImageNumber* imnumber;
	int x, y, w, h;
public:
	NumWithEffect(FilePath path,String numStr, int fixedAtTime, int x, int y, int w, int h);
	~NumWithEffect(void);
	bool update(double msF);
	void draw(void);
};