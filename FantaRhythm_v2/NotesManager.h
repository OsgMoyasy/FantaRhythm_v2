#pragma once
# include <Siv3D.hpp> // OpenSiv3D v0.3.2
#include "Game.h"
#include "Observer.h"
#include <vector>
#include <list>
#include <array>
#include "Judge.h"

constexpr int LANESIZE = 4;		//LANE�̐�
constexpr int BAD_RANGE = 200;//����̍ő�͈�[ms]��2
constexpr int GOOD_RANGE = 100;//GOOD����͈�[ms]��2
constexpr int GREAT_RANGE = 25;//GREAT����͈�[ms]��2
constexpr int PERFECT_RANGE = 5;//PERFECT����͈�[ms]��2




/*--------------------------------------------


-----------------------------------------------*/
class NotesManager {
private:
	enum NOTESTYPE {
		NORMAL,
		LONG,
		SENTINEL,
	};
	
	typedef struct _Notes {
		NOTESTYPE type;
		int time;
		int longtime;
		bool display;
	}Notes;

	int notewidth;
	
	class NotesSubject* notessubject;
	
	std::list<Notes> notelist[LANESIZE];
	using noteitr = std::list<Notes>::iterator;
	noteitr checkitr[LANESIZE];		//���肷�ׂ��C�e���[�^�ێ�
	noteitr displayitr[LANESIZE];	//�\�����ׂ��C�e���[�^�ێ�

	JUDGE::JudgeCount judgecount;

	int nowTime;	//���ݎ���[ms]

	bool down[LANESIZE];	//�����I�ɂ�bool�ł͂Ȃ��Ȃ艟���ꂽ�u�ԁA�����ꂽ�u�Ԃ̍Đ�����������
	bool press[LANESIZE];	//������bool�ŉ�����Ă��ԗ�����Ă��Ԃ��Ǘ�

	bool longflag[LANESIZE];//������Ă��邩�ǂ�����Ԃ�ێ�

	//�`��֌W�̕ϐ�
	int laneStartX[LANESIZE];	//Start = ���[���̏�[
	int laneStartY;		
	int laneJudgeX[LANESIZE];	//Judge = ���[���Ɣ����������鏊
	int laneJudgeY;
	int laneGoalY;		//Goal = ����؂����m�[�c�̕\������߂鏊
	float timeRequired;	//�m�[�c�̏o�����画��܂ŗ���鎞��[ms]
	float notespeed;	//�m�[�c���x�̕␳�{��

	void plusItr(noteitr& itr);	//notelist�̃C�e���[�^��i�߂�
	void checkAttack(void);		//�{�^���̉����󋵂��m�F����

	void controlJudge(void);	//�m�[�c�̎�ޖ��ɔ���p�֐����Ăяo��
	void judgeNormal(int lane);	//�m�[�}���m�[�c�𔻒�
	void judgeLong(int lane);	//�����O�m�[�c�𔻒�

	JUDGE::TYPE judgeType(int checktime);//����̃^�C�v��Ԃ�

	void judgeEvent(JUDGE::TYPE type, int lane);
	void judgeLongEvent(JUDGE::TYPE type, int lane);
	
	double getProgress(int time);//���[����[���画����܂ł̐i�񂾊�����Ԃ�
	double progressByAngle(double progressRate);//���[���̊p�x�ɂ��␳��progressRate�ɍs��
	double getCurrentPosition(int startPos, int endPos, double progressRate);//���ݍ��W��Ԃ�
	double getScale(double currenty);//�g�嗦�v�Z

	void displayNormal(int lane, int time);				//�m�[�}���m�[�c��\��
	void displayLong(int lane, int time, int longtime);	//�����O�m�[�c��\��

	void setEvent(Massage msg, int val);
	
public:
	NotesManager(NotesSubject* sub,const String& difpath);
	~NotesManager();
	void update(void);
	void draw(void);
	JUDGE::JudgeCount* getJudgeCount(void);
};