#pragma once
# include <Siv3D.hpp> // OpenSiv3D v0.3.2
#include <vector>
#include <list>
#include <array>
#include "Observer.h"
#include "Judge.h"
#include "GameEffect.h"
#include "MusicManager.h"
#include "MyKey.h"
#include "JudgeEffect.h"
#include "ImageNumber.h"


class NotesManager {
private:
	static constexpr int LANESIZE = 4;
	enum class NOTESTYPE;
	struct Notes;

	class NotesSubject* notessubject;
	class JudgeEffect* judgeEffect;
	class ComboImNumber* comboImNum;
	
	std::list<Notes> notelist[LANESIZE];
	using noteitr = std::list<Notes>::iterator;
	noteitr checkitr[LANESIZE];		//判定すべきイテレータ保持
	noteitr displayitr[LANESIZE];	//表示すべきイテレータ保持

	JUDGE::JudgeCount judgecount;
	class Combo {
	public:
		Combo();
		void add(int val = 1);
		void reset();
		int get();
		int getMax();
	private:
		int count;//現在コンボ数
		int max;//マックスコンボ数
	} combo;

	int nowtime;	//現在時間[ms]

	//ボタン関係の変数
    int down[LANESIZE];
	int press[LANESIZE];

	//ロングノーツ用
	int pressedkey[LANESIZE];//押されたボタンを保持

	//描画関係の変数
	FlipSet<JUDGE::TYPE> effect;//使用するエフェクト
	int laneStartX[LANESIZE];	//Start = レーンの上端
	int laneStartY[LANESIZE];
	int laneJudgeX[LANESIZE];	//Judge = レーンと判定線が交わる所
	int laneJudgeY[LANESIZE];
	int laneGoalY;		//Goal = 流れ切ったノーツの表示をやめる所
	float timeRequired;	//ノーツの出現から判定まで流れる時間[ms]
	float notespeed;	//ノーツ速度の補正倍率
	int notewidth;
	float laneStartScale;	//ノーツの拡大率
	float laneJudgeScale;

	void plusItr(noteitr& itr);	//notelistのイテレータを進める

	void checkAttack(void);		//ボタンの押し状況を確認する
	struct ButtonandJudge;		//NoteisHit関数の応答用
	JUDGE::TYPE NoteisHit(int judgetime);
	JUDGE::TYPE judgeType(int checktime);//判定のタイプを返す

	void controlJudge(void);		//ノーツの種類毎に判定用関数を呼び出し
	void judgeNormal(int lane);		//ノーマルノーツを判定
	void judgeLong(int lane);		//ロングノーツを判定
	void judgeCritical(int lane);	//敵の攻撃ノーツを判定

	void judgeEvent(JUDGE::TYPE type, int lane, bool next = true);
	void judgeLongEvent(JUDGE::TYPE type, int lane);
	void judgeCriticalEvent(JUDGE::TYPE type, int lane, int buttonType);
	void noteNext(int lane);
	
	void displayLane(int lane);		//レーンを表示
	void displayButton(int lane);	//ボタン押し状況 兼 判定円 を表示

	double getProgress(int time);													//レーン上端から判定線までの進んだ割合を返す
	double progressByAngle(double progressRate);									//レーンの角度による補正をprogressRateに行う
	double getCurrentPosition(double startPos, double endPos, double progressRate);	//現在座標を返す
	struct ProPos;							//progressRateと現在座標(X,Y)
	ProPos getProPos(int lane, int time);	//progressRateと現在座標(X,Y)を返す

	void displayNormal(int lane, int time, float shadow);				//ノーマルノーツを表示
	void displayLong(int lane, int time, int longtime, float shadow);	//ロングノーツを表示
	Color longLineColor(int linex, int lane, int time);					//ロングノーツの色を決定
	void displayCritical(int lane, int time, float shadow);				//敵の攻撃ノーツを表示
	void playNotesEffect(ProPos pos, JUDGE::TYPE type);	//ノーツ破壊時のエフェクト再生

	void setEvent(Massage msg, int val);
	
public:
	NotesManager(NotesSubject* sub,const String& difpath,float _notespeed);
	~NotesManager();
	bool isReady();
	void update(void);
	void draw(void);
	JUDGE::JudgeCount* getJudgeCount(void);
};
