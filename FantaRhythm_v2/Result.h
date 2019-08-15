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
	JUDGE::JudgeCount *judgecount;
	bool clearflag;
	//ゲームクリア用
	void gameclearUpdate(void);
	void gameclearDraw(void);
	//ゲームオーバー用
	void gameoverUpdate(void);	
	void gameoverDraw(void);

};
