#pragma once
#include "Scene.h"
#include "Judge.h"
#include "GameEffect.h"
#include <Siv3D.hpp>


class Result : public Scene {
public:
	Result(JUDGE::JudgeCount judgeCnt, int totalDamage, bool clearFlag);
	~Result(void);
	void update(void);
	void draw(void);

private:
	class SE* se;
	class FlipEffect* feffect;
	Texture imNumber[10];

	int totalDamage;
	JUDGE::JudgeCount judgeCnt;
	bool clearFlag;

	int framecnt;
	static const int alphatime = 3 * 60;//アルファ値が元に戻るまでの時間×フレーム数
	double alphaBack;
	double alphaFont;

	void(Result::* stateUpdate)(void);//実行する計算処理への関数ポインタ
	void(Result::* stateDraw)(void);//実行する描画処理への関数ポインタ

	void changeFontAlpha(void);//サイクルの周期でフォントの透明度変更

	//ゲームクリア用

	String scoreStr;//スコアを文字列変換
	String scoreDraw;//実際に描画するスコア

	void imNumberInit(void);
	void scoreEffect(void); //スコアを0~9と順番に変わって下位の桁から確定させるようにする
	int calcScore(JUDGE::JudgeCount &judgeCnt);//最終的なスコア計算
	void successUpdate(void);
	void successDraw(void);

	//ゲームオーバー用
	void failedUpdate(void);
	void failedDraw(void);
};
