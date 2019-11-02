#pragma once
#include "Scene.h"
#include <Siv3D.hpp>
#include "SceneManager.h"
#include "MyKey.h"


class SelectMusic:public Scene{
public:
	SelectMusic(void);
	~SelectMusic(void);	
	bool isReady(void);
	void start(void);
	void update(void);
	void draw(void);

	String getMusicPath(void);
	String getDifPath(void);
	float getNoteSpeed(void);
private:
	enum SELECTSTATE {
		MUSIC,
		DIFFICULTY,
		QRREAD,
		GAME
	};

	Audio* audio;

	//それぞれの要素数、現在選択している要素添字、移動用の値
	int musiccount, difcount;
	int musiccursor, difcursor;
	int musicrotation, difrotation;
	float notespeed;

	s3d::Array<FilePath> musicarray;
	s3d::Array<FilePath> difarray;

	void(SelectMusic::*stateUpdate)(void);//実行する計算処理への関数ポインタ
	void(SelectMusic::*stateDraw)(void);//実行する描画処理への関数ポインタ

	void changeState(SELECTSTATE nextstate);//現在の状態を変更する

	//初期化処理
	void initDifficulty(void);
	void initMusic(void);
	//初期化で使用する関数
	String getDiffilepath(int musiccursor);
	void setArray(s3d::Array<FilePath>&array, const FilePath& filepath, int& count);
	//s計算処理
	void updateMusic(void);
	void updateDifficulty(void);
	//描画処理
	void drawMusic(void);
	void drawDifficulty(void);
	//上下移動検知処理
	void difmoveCursor(void);
	void musicmoveCursor(void);
	//移動処理
	void rotatemusic(int& rotation);
	//プレビュー再生
	void playMusic(int musicNum);
};

