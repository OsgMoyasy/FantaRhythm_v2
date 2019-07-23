#pragma once
#include "Game.h"
#include "FantaRhythm_v2.h"
#include "Observer.h"
#include <vector>
#include <list>

#define LANESIZE 4		//LANEの数

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
	std::list<Notes>::iterator checkitr[LANESIZE];//判定すべきイテレータ保持
	std::list<Notes>::iterator displayitr[LANESIZE];//表示すべきイテレータ保持

	bool down[4];//将来的にはboolではなくなり押された瞬間、離された瞬間の再生時刻が入る
	bool press[4];//ここはboolで押されてる状態離されてる状態を管理

	bool longflag[4];

	int laneStartX[4];
	int laneJudgeX[4];
	int laneStartY;
	int laneJudgeY;//CHECKLINEYの代わり
	int laneGoalY;//画面Y座標限界+10
	float timeRequired;//ノーツの出現から判定まで流れる時間[ms]
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