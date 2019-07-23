#pragma once
#include "Game.h"
#include "FantaRhythm_v2.h"
#include "Observer.h"
#include <vector>
#include <list>

#define LANESIZE 4		//LANE�̐�

/*--------------------------------------------


-----------------------------------------------*/
class NotesManager {
private:
	enum NOTESTYPE {
		NORMAL,
		LONG,
		SENTINEL
	};
	typedef struct _Notes {
		NOTESTYPE type;
		int time;
		int longtime;
		bool display;
	}Notes;

	int notewidth;
	
	float notespeed;

	class NotesSubject* subject;
	
	std::list<Notes> notelist[LANESIZE];
	std::list<Notes>::iterator checkitr[LANESIZE];//���肷�ׂ��C�e���[�^�ێ�
	std::list<Notes>::iterator displayitr[LANESIZE];//�\�����ׂ��C�e���[�^�ێ�

	bool down[4];//�����I�ɂ�bool�ł͂Ȃ��Ȃ艟���ꂽ�u�ԁA�����ꂽ�u�Ԃ̍Đ�����������
	bool press[4];//������bool�ŉ�����Ă��ԗ�����Ă��Ԃ��Ǘ�

	bool longflag[4];

	int laneStartX[4];
	int laneJudgeX[4];
	int laneStartY;
	int laneJudgeY;//CHECKLINEY�̑���
	int laneGoalY;//���Y���W���E+10
	float timeRequired;//�m�[�c�̏o�����画��܂ŗ���鎞��[ms]
	double nowTime;

	void plusItr(std::list<Notes>::iterator& itr);
	void checkAttack(void);
	void controlJudge(void);
	void judgeNormal(int lane);
	void judgeLong(int lane);
	void displayNormal(int lane, int time);
	void displayLong(int lane, int time, int longtime);

	double getProgress(int time);
	int getCurrentPosition(int startPos, int endPos, double progressRate);
	double getScale(double currenty);

	void setEvent(Massage msg, int val);
	
public:
	NotesManager(NotesSubject* sub,const String& difpath);
	~NotesManager();
	void update(void);
	void draw(void);

};