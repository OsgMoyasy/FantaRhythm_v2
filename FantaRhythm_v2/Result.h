#pragma once
#include "FantaRhythm_v2.h"
#include "Scene.h"
#include "Judge.h"
#include <Siv3D.hpp>



class Result : public Scene {
public:
	Result(JUDGE::JudgeCount judgecnt, int totaldmg, bool cflag);
	~Result(void);
	void update(void);
	void draw(void);

private:
	int totaldamage;
	JUDGE::JudgeCount judgecount;
	bool clearflag;

	int framecnt;
	static const int alphatime = 3 * 60;//アルファ値が元に戻るまでの時間×フレーム数
	double alpha;
	double alphafont;

	void(Result::* stateUpdate)(void);//実行する計算処理への関数ポインタ
	void(Result::* stateDraw)(void);//実行する描画処理への関数ポインタ

	//ゲームクリア用
	String scoreStr;//スコアを数値変換したもの
	String scoreDraw;//実際に描画するスコア

	void scoreEffect(void);



	int calcScore(JUDGE::JudgeCount &jc);//スコア計算
	void successUpdate(void);
	void successDraw(void);
	//ゲームオーバー用
	void failedUpdate(void);
	void failedDraw(void);
};
