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
	static const int alphatime = 3 * 60;//アルファ値が元に戻るまでの時間×フレーム数
	double alpha;
	double alphafont;

	void(Result::* stateUpdate)(void);//実行する計算処理への関数ポインタ
	void(Result::* stateDraw)(void);//実行する描画処理への関数ポインタ

	//ゲームクリア用
	int score;


	void calcScore(void);//スコア計算
	void successUpdate(void);
	void successDraw(void);
	//ゲームオーバー用
	void failedUpdate(void);
	void failedDraw(void);
};
