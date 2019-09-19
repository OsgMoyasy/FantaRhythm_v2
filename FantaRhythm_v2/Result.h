#pragma once
#include "Scene.h"
#include "Judge.h"
#include "GameEffect.h"
#include <Siv3D.hpp>


class Result : public Scene {
public:
	Result(JUDGE::JudgeCount judgeCnt, int totalDamage, bool isClear);
	~Result(void);
	void update(void);
	void draw(void);

private:
	Stopwatch stopwatch;
	class SE* se;
	class ImageNumber* judgeImNum;
	class NumWithEffect* scoreNumEffect;
	class NumWithEffect* damageNumEffect;
	
	int score;
	int totalDamage;
	JUDGE::JudgeCount judgeCnt;
	bool isClear;

	int framecnt;
	static const int alphatime = 3 * 60;//アルファ値が元に戻るまでの時間×フレーム数
	double alphaBack;
	double alphaFont;

	void(Result::* stateUpdate)(void);//実行する計算処理への関数ポインタ
	void(Result::* stateDraw)(void);//実行する描画処理への関数ポインタ

	void changeFontAlpha(void);//サイクルの周期でフォントの透明度変更

	//ゲームクリア用

	String scoreStr;//スコアを文字列変換
	String damageStr;//ダメージを文字列変換

	void successUpdate(void);
	void successDraw(void);

	int calcScore(JUDGE::JudgeCount& judgeCnt);//スコア計算
	void imNumberInit(void);//画像の数字に関する初期化　※使用する文字列の初期化より後に呼び出すこと
	bool judgeUpdate();//判定数を順番に下方向に描画していくための関数
	

	//ゲームオーバー用
	void failedUpdate(void);
	void failedDraw(void);
};


class ImageNumber {
private:
	Texture imNumber[10];
	int imw, imh;

	typedef struct _NumPoint {
		int x, y;
		int num;
	}NumPoint;

	std::vector<NumPoint> numberp;//追加された数字を全て保持


public:
	ImageNumber(FilePath path, int w, int h);
	~ImageNumber();
	void addOne(int num, int x, int y);//数字を１文字追加する
	void ImageNumber::addMulti(int num, int x, int y);//数字列を追加する
	void draw();

};

class NumWithEffect {
private:
	String numStr;
	class FlipEffect* numEffect;
	int fixedAtTime;
	int currentWord;//現在確定させようとしている数字
	double prevtime;
	ImageNumber* imnumber;
	int x, y, w, h;
public:
	NumWithEffect(FilePath path,String numStr, int fixedAtTime, int x, int y, int w, int h);
	~NumWithEffect(void);
	bool update(double msF);
	void draw(void);
};