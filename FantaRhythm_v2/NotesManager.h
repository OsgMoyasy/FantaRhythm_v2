#pragma once
#include "Game.h"
#include "FantaRhythm_v2.h"
#include <vector>
#include <list>

#define LANESIZE 4		//LANEの数

/*--------------------------------------------


-----------------------------------------------*/
class NotesManager {
private:
	enum NOTESTYPE {
		NORMAL,
		LONG
	};
	typedef struct _Notes {
		NOTESTYPE type;
		int time;
		int longtime;
		bool display;
	}Notes;
	
	float notespeed;

	//float checktime[4] = { 10,30,50,200 };
	
	std::list<Notes> notelist[LANESIZE];
	std::list<Notes>::iterator checkitr[LANESIZE];//判定すべきイテレータ保持
	std::list<Notes>::iterator displayitr[LANESIZE];//表示すべきイテレータ保持

	bool down[4];
	bool press[4];

	bool longflag[4];

	int laneStartX[4];
	int laneJudgeX[4];
	int laneStartY;
	int laneJudgeY;//CHECKLINEYの代わり
	int laneGoalY;//画面Y座標限界+10
	float timeRequired;//ノーツの出現から判定まで流れる時間[ms]
	double nowTime;

	void plusItr(int i, std::list<Notes>::iterator& itr);
	void checkAttack(void);
	void controlJudge(void);
	void judgeNormal(int lane);
	void judgeLong(int lane);
	void displayNormal(int lane, std::list<Notes>::iterator& itr);
	void displayLong(int lane, std::list<Notes>::iterator& itr);
	
public:
	NotesManager(const String& difpath);
	~NotesManager();
	void update(void);
	void draw(void);

};