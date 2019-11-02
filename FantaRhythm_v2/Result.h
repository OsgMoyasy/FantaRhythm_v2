#pragma once
#include "Scene.h"
#include "Judge.h"
#include "ImageNumber.h"
#include "GameEffect.h"
#include <Siv3D.hpp>
#include "MyKey.h"
#include "SceneManager.h"
#include "HttpClient.h"
#include <thread>
#include "RankingData.h"
class Result : public Scene {
public:
	Result(JUDGE::JudgeCount judgeCnt, int totalDamage, bool isClear);
	~Result(void);
	bool isReady(void);
	void start(void);
	void update(void);
	void draw(void);

private:
	Stopwatch stopwatch;
	class SE* se;
	class NumWithEffect* scoreNumEffect;
	class NumWithEffect* damageNumEffect;
	class ImageNumber* judgeImNum[JUDGE::TYPE_SIZE + 1];
	
	int score;
	int totalDamage;
	JUDGE::JudgeCount judgeCnt;
	bool isClear;
	bool isEffectEnd;
	
	double alphaBack;
	double alphaFont;

	void(Result::* stateUpdate)(void);//���s����v�Z�����ւ̊֐��|�C���^
	void(Result::* stateDraw)(void);//���s����`�揈���ւ̊֐��|�C���^

	void changeFontAlpha(void);//�T�C�N���̎����Ńt�H���g�̓����x�ύX

	//�Q�[���N���A�p

	String scoreStr;//�X�R�A�𕶎���ϊ�
	String damageStr;//�_���[�W�𕶎���ϊ�
	int judgeDrawRow;//���萔�`��̌��݂̑Ώ�
	double judgePrevTime;
	std::thread th;
	TH_STATUS th_status;
	HttpClient* client;

	void successUpdate(void);
	void successDraw(void);


	int calcScore(JUDGE::JudgeCount& judgeCnt);//�X�R�A�v�Z
	void imNumberInit(void);//�摜�̐����Ɋւ��鏉�����@���g�p���镶����̏���������ɌĂяo������
	bool judgeUpdate();//���萔�����Ԃɉ������ɕ`�悵�Ă������߂̊֐�
	
	//�Q�[���I�[�o�[�p
	void failedUpdate(void);
	void failedDraw(void);

};


