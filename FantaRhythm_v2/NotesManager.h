#pragma once
# include <Siv3D.hpp> // OpenSiv3D v0.3.2
#include <vector>
#include <list>
#include <array>
#include "Observer.h"
#include "Judge.h"
#include "GameEffect.h"
#include "MusicManager.h"

class NotesManager {
private:
	static constexpr int LANESIZE = 4;
	enum class NOTESTYPE;
	struct Notes;

	/*
	class Update {
	public:
		Update();
		~Update();
		void update();
		void operator()() { update(); }
	private:
	};

	class Draw {
	public:
		Draw();
		~Draw();
		void draw();
		void operator()() { draw(); }
	private:
	};
	*/
	int notewidth;
	
	class NotesSubject* notessubject;
	
	std::list<Notes> notelist[LANESIZE];
	using noteitr = std::list<Notes>::iterator;
	noteitr checkitr[LANESIZE];		//判定すべきイテレータ保持
	noteitr displayitr[LANESIZE];	//表示すべきイテレータ保持

	JUDGE::JudgeCount judgecount;

	int nowtime;	//現在時間[ms]

	//ボタン関係の変数
	bool down[LANESIZE][2];		//将来的にはboolではなくなり押された瞬間、離された瞬間の再生時刻が入る
	bool press[LANESIZE][2];	//ここはboolで押されてる状態離されてる状態を管理。[2]は縦に並んだ2つのボタンを表し、[0]は上、[1]は下である。
	enum class PSHBTN;		//縦列のボタンの押し状況(無し,上,下,同時)

	//ロングノーツ用
	bool longflag[LANESIZE];//押されているかどうか状態を保持
	PSHBTN pressedkey[LANESIZE];//押されたボタンを保持

	//描画関係の変数
	int laneStartX[LANESIZE];	//Start = レーンの上端
	int laneStartY;		
	int laneJudgeX[LANESIZE];	//Judge = レーンと判定線が交わる所
	int laneJudgeY;
	int laneGoalY;		//Goal = 流れ切ったノーツの表示をやめる所
	float timeRequired;	//ノーツの出現から判定まで流れる時間[ms]
	float notespeed;	//ノーツ速度の補正倍率

	void plusItr(noteitr& itr);	//notelistのイテレータを進める

	void checkAttack(void);		//ボタンの押し状況を確認する
	struct ButtonandJudge;		//NoteisHit関数の応答用
	JUDGE::TYPE judgeType(int checktime);//判定のタイプを返す
	ButtonandJudge NoteisHit(int lane, int judgetime);
	bool NoteisPress(int lane, PSHBTN button);//上下ボタンを指定して

	void controlJudge(void);	//ノーツの種類毎に判定用関数を呼び出し
	void judgeNormal(int lane);	//ノーマルノーツを判定
	void judgeLong(int lane);	//ロングノーツを判定


	void judgeEvent(JUDGE::TYPE type, int lane);
	void judgeLongEvent(JUDGE::TYPE type, int lane);
	
	double getProgress(int time);//レーン上端から判定線までの進んだ割合を返す
	double progressByAngle(double progressRate);//レーンの角度による補正をprogressRateに行う
	double getCurrentPosition(int startPos, int endPos, double progressRate);//現在座標を返す
	double getScale(double currenty);//拡大率計算

	void displayNormal(int lane, int time);				//ノーマルノーツを表示
	void displayLong(int lane, int time, int longtime);	//ロングノーツを表示

	enum USE_EFFECT;//useFlipEffectの添字に使用
	Array<FlipEffect> useFlipEffect;
//	void effectInit();//使用するエフェクトを登録
//	void drawAllEffect();//再生中の全てのFlipEffectを描画

	void setEvent(Massage msg, int val);
	
public:
	NotesManager(NotesSubject* sub,const String& difpath);
	~NotesManager();
	void update(void);
	void draw(void);
	JUDGE::JudgeCount* getJudgeCount(void);
};
