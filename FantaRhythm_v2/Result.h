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

	void(Result::* stateUpdate)(void);//実行する計算処理への関数ポインタ
	void(Result::* stateDraw)(void);//実行する描画処理への関数ポインタ

	void changeFontAlpha(void);//サイクルの周期でフォントの透明度変更

	//ゲームクリア用

	String scoreStr;//スコアを文字列変換
	String damageStr;//ダメージを文字列変換
	int judgeDrawRow;//判定数描画の現在の対象
	double judgePrevTime;
	std::thread th;
	TH_STATUS th_status;
	HttpClient* client;

	void successUpdate(void);
	void successDraw(void);


	int calcScore(JUDGE::JudgeCount& judgeCnt);//スコア計算
	void imNumberInit(void);//画像の数字に関する初期化　※使用する文字列の初期化より後に呼び出すこと
	bool judgeUpdate();//判定数を順番に下方向に描画していくための関数
	
	//ゲームオーバー用
	void failedUpdate(void);
	void failedDraw(void);

};


