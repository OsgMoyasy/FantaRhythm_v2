#pragma once
# include <Siv3D.hpp> // OpenSiv3D v0.3.2
#include "Game.h"
#include "Observer.h"
#include <vector>
#include <list>
#include <array>
#include "Judge.h"

constexpr int LANESIZE = 4;		//LANEの数
constexpr int BAD_RANGE = 200;//判定の最大範囲[ms]÷2
constexpr int GOOD_RANGE = 100;//GOOD判定範囲[ms]÷2
constexpr int GREAT_RANGE = 25;//GREAT判定範囲[ms]÷2
constexpr int PERFECT_RANGE = 5;//PERFECT判定範囲[ms]÷2




/*--------------------------------------------


-----------------------------------------------*/
class NotesManager {
private:
	enum NOTESTYPE {
		NORMAL,
		LONG,
		SENTINEL,
	};
	
	typedef struct _Notes {
		NOTESTYPE type;
		int time;
		int longtime;
		bool display;
	}Notes;

	int notewidth;
	
	class NotesSubject* notessubject;
	
	std::list<Notes> notelist[LANESIZE];
	using noteitr = std::list<Notes>::iterator;
	noteitr checkitr[LANESIZE];		//判定すべきイテレータ保持
	noteitr displayitr[LANESIZE];	//表示すべきイテレータ保持

	JUDGE::JudgeCount judgecount;

	int nowTime;	//現在時間[ms]

	bool down[LANESIZE];	//将来的にはboolではなくなり押された瞬間、離された瞬間の再生時刻が入る
	bool press[LANESIZE];	//ここはboolで押されてる状態離されてる状態を管理

	bool longflag[LANESIZE];//押されているかどうか状態を保持

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

	void controlJudge(void);	//ノーツの種類毎に判定用関数を呼び出し
	void judgeNormal(int lane);	//ノーマルノーツを判定
	void judgeLong(int lane);	//ロングノーツを判定

	JUDGE::TYPE judgeType(int checktime);//判定のタイプを返す

	void judgeEvent(JUDGE::TYPE type, int lane);
	void judgeLongEvent(JUDGE::TYPE type, int lane);
	
	double getProgress(int time);//レーン上端から判定線までの進んだ割合を返す
	double progressByAngle(double progressRate);//レーンの角度による補正をprogressRateに行う
	double getCurrentPosition(int startPos, int endPos, double progressRate);//現在座標を返す
	double getScale(double currenty);//拡大率計算

	void displayNormal(int lane, int time);				//ノーマルノーツを表示
	void displayLong(int lane, int time, int longtime);	//ロングノーツを表示

	void setEvent(Massage msg, int val);
	
public:
	NotesManager(NotesSubject* sub,const String& difpath);
	~NotesManager();
	void update(void);
	void draw(void);
	JUDGE::JudgeCount* getJudgeCount(void);
};