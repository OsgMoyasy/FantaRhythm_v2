#include "NotesManager.h"


enum class NotesManager::NOTESTYPE {
	NORMAL,
	LONG,
	SENTINEL,
};

namespace JUDGE_RANGE{
	constexpr int BAD = 200;//判定の最大範囲[ms]÷2
	constexpr int GOOD = 100;//GOOD判定範囲[ms]÷2
	constexpr int GREAT = 25;//GREAT判定範囲[ms]÷2
	constexpr int PERFECT = 5;//PERFECT判定範囲[ms]÷2
}

struct NotesManager::Notes {
	NOTESTYPE type;
	int time;
	int longtime;
	bool display;
};

enum class NotesManager::PSHBTN {
	NONE,
	UP,
	DOWN,
	BOTH,
};

struct NotesManager::ButtonandJudge {
	PSHBTN button;
	JUDGE::TYPE judge;
};

NotesManager::NotesManager(NotesSubject* sub, const String& difpath) {
//	Update* update = new Update;
//	Draw* draw = new Draw;

	TextureAsset::Register(U"note", U"resources/images/items/Nort3rd.png");
	TextureAsset::Preload(U"note");
//	effectInit();

	CSVData csv;//譜面の取得　多次元配列で管理 0 判定時間(ms) 1 長さ？ 2 流すレーン[0-3]
	Print << difpath;

	notessubject = sub;//サブジェクトの登録

	csv.load(difpath);//譜面のロード
	NotesManager::Notes note;
	for (int row = 0; row < csv.rows(); row++) {
		note.time = csv.get<int>(row, 0);
		note.longtime = note.time + csv.get<int>(row, 1);
		int lane = csv.get<int>(row, 2);
		switch (csv.get<int>(row, 3)) {
			case 0:
				note.type = NOTESTYPE::NORMAL;
				break;
			case 1:
				note.type = NOTESTYPE::LONG;
				break;
		}
		note.display = true;
		notelist[lane].push_back(note);
	}
	note.type = NOTESTYPE::SENTINEL;//番兵の追加
	note.time = 0;
	note.longtime = 0;
	note.display = false;

	for (int i = 0; i < JUDGE::TYPE::TYPE_SIZE; i++) {
		judgecount.cnt[i] = 0;
	}
	for (int lane = 0; lane < LANESIZE; lane++) {//レーンごとに到着時間を格納
		notelist[lane].push_back(note);//番兵の設置
		displayitr[lane] = checkitr[lane] = notelist[lane].begin();//チェック用のイテレータ初期
		longflag[lane] = false;
	}

	//描画関係の変数の初期化
	//X座標
	int Center = Window::Width() / 2;//レーン群の中心
	int StartBetween = 80;//上端でのレーン間の距離
	int JudgeBetween = 140;//判定線でのレーン間の距離
	for (int i = 0;i < LANESIZE;i++) {
		laneStartX[i] = Center + (int)((i - (LANESIZE - 1) / 2.0) * StartBetween);
		laneJudgeX[i] = Center + (int)((i - (LANESIZE - 1) / 2.0) * JudgeBetween);
	}
	//Y座標
	laneStartY = 100;
	laneJudgeY = 650;
	laneGoalY = 730;//(画面Y座標限界 + 10)
	//速さ
	notespeed = 1.0;
	timeRequired = 1500 / notespeed;

	notewidth = TextureAsset(U"note").width();
}
NotesManager::~NotesManager() {
	TextureAsset::UnregisterAll();
}


void NotesManager::update(void)
{
	nowtime = (int)(MusicManager::getMusicTime());
	checkAttack();
	controlJudge();
}

void NotesManager::plusItr(noteitr& itr) {
	if (itr->type != NOTESTYPE::SENTINEL)//番兵かどうか判定
		itr++;
}


void NotesManager::checkAttack(void) {
	down[0][0] = KeyQ.down();press[0][0] = KeyQ.pressed();
	down[0][1] = KeyA.down();press[0][1] = KeyA.pressed();

	down[1][0] = KeyW.down();press[1][0] = KeyW.pressed();
	down[1][1] = KeyS.down();press[1][1] = KeyS.pressed();

	down[2][0] = KeyE.down();press[2][0] = KeyE.pressed();
	down[2][1] = KeyD.down();press[2][1] = KeyD.pressed();

	down[3][0] = KeyR.down();press[3][0] = KeyR.pressed();
	down[3][1] = KeyF.down();press[3][1] = KeyF.pressed();
}
JUDGE::TYPE NotesManager::judgeType(int checktime) {//判定のタイプを返す
	if (checktime <= JUDGE_RANGE::PERFECT) {//PERFECT
		return JUDGE::PERFECT;
	}
	else if (checktime <= JUDGE_RANGE::GREAT) {//GREAT
		return JUDGE::GREAT;
	}
	else if (checktime <= JUDGE_RANGE::GOOD) {//GOOD
		return JUDGE::GOOD;
	}
	else if (checktime <= JUDGE_RANGE::BAD) {//BAD
		return JUDGE::BAD;
	}
	else {//NONE
		return JUDGE::NONE;
	}
}
NotesManager::ButtonandJudge NotesManager::NoteisHit(int lane, int judgetime) {
	constexpr int BUTTONSIZE = 2;//一列のボタンの個数
	int keysum = 0;//押されたキー
	JUDGE::TYPE tmp = JUDGE::NONE;
	JUDGE::TYPE judge = JUDGE::NONE;
	int checktime = abs(nowtime - judgetime);

	for (int i = 0;i < BUTTONSIZE;i++) {
		if (
			down[lane][i] == true&&//ボタンが押された且つ
			(tmp = judgeType(checktime)) < JUDGE::NONE//判定時間内なら
			) {
			keysum += i+1;//ボタンの上下に対応した数を足す。
			judge = tmp < judge ? tmp : judge;//より判定の良い方を格納
		}
	}
	return { (PSHBTN)keysum,judge };//PSHBTNenumの並び順に依存している。
}
bool NotesManager::NoteisPress(int lane, PSHBTN button) {
	constexpr int BUTTONSIZE = 2;//一列のボタンの個数
	int keysum = (int)button;
	
	for (int i = BUTTONSIZE;i >= 1;i--) {
		if (i <= keysum&&press[lane][i-1]) {
			keysum -= i;
		}
	}
	return !keysum;//keysumが残っていたらfalse(指定されたボタンが押されていない)
}

void NotesManager::controlJudge(void) {
	for (int i = 0;i < LANESIZE;i++) {
		switch (checkitr[i]->type) {
		case NOTESTYPE::NORMAL:
			judgeNormal(i);
			break;
		case NOTESTYPE::LONG:
			judgeLong(i);
			break;
		default:
			break;
		}
	}
}
void NotesManager::judgeNormal(int lane) {
	ButtonandJudge bandj = NoteisHit(lane, checkitr[lane]->time);

	if (bandj.button != PSHBTN::NONE) {//押されてるかつ判定時間内なら判定処理
		return judgeEvent(bandj.judge, lane);
	}
	else if (nowtime >= checkitr[lane]->time + JUDGE_RANGE::BAD) {//押されてないまま終了時
		return judgeEvent(JUDGE::BAD, lane);
	}
}
void NotesManager::judgeLong(int lane) {
	if (longflag[lane] == false) {
		ButtonandJudge bandj = NoteisHit(lane, checkitr[lane]->time);
		if (bandj.judge <= JUDGE::GOOD) {//下端で押されたら(この判定はJUDGEenumの並び順に依存している)
			longflag[lane] = true;//そのロングノーツの判定を有効化
			pressedkey[lane] = bandj.button;//その時押されたボタンを記憶
		}
	}

	if (longflag[lane] == true) {//離すときの処理
		if (NoteisPress(lane, pressedkey[lane]) == true) {//ボタン押下中
			checkitr[lane]->time = (int)(nowtime);//判定位置以降で下側を止める
			if (nowtime >= checkitr[lane]->longtime) {//押されているままノーツの上端を過ぎた時
				return judgeLongEvent(JUDGE::GOOD, lane);//強制的に次のノーツへ処理を移行
			}
		}
		else {//離した
			ButtonandJudge bandj = NoteisHit(lane, checkitr[lane]->longtime);
			return judgeLongEvent(bandj.judge, lane);
		}
	}
	
	if (nowtime >= checkitr[lane]->longtime + JUDGE_RANGE::GOOD) {//判定を超えた時
		return judgeLongEvent(JUDGE::BAD, lane);
	}
}
void NotesManager::judgeLongEvent(JUDGE::TYPE type, int lane) {
	judgeEvent(type, lane);
	longflag[lane] = false;//判定したので長押しの状態を初期化
}

void NotesManager::judgeEvent(JUDGE::TYPE type, int lane) {
	checkitr[lane]->display = false;//ディスプレイ表示オフ
	plusItr(checkitr[lane]);//判定対象を次に進める
	judgecount.cnt[type]++;//判定をカウントアップ
	if(type == JUDGE::BAD){
		setEvent(Massage::DAMAGE, lane);
	}
	else {
		setEvent(Massage::UPATTACK, lane);
	}
}
JUDGE::JudgeCount* NotesManager::getJudgeCount() {
	return &judgecount;
}


void NotesManager::draw(void){

	//デバッグ用
	for (int i = 0; i < 4; i++) {
		Print << judgecount.cnt[i];
	}
	Line(0, laneJudgeY, 1920, laneJudgeY).draw(3, Palette::Black);	//判定線の描画

	for(int i = 0; i < LANESIZE; i++){
		Line(laneStartX[i], laneStartY, laneJudgeX[i], laneJudgeY).draw(1, Palette::Red);	//レーンの描画
		for (noteitr itr = displayitr[i]; itr != notelist[i].end(); itr++) {
			if (nowtime < itr->time - timeRequired)//描画前なら描画打ち切り
				break;
			if (itr->display == false)
				continue;

			switch (itr->type){
			case NOTESTYPE::NORMAL:
				displayNormal(i, itr->time);
				break;
			case NOTESTYPE::LONG:
				displayLong(i, itr->time, itr->longtime);
				break;
			default:
				break;
			}
		}	
	}
//	drawAllEffect();
}

double NotesManager::getProgress(int time) {
	return (timeRequired - (time - nowtime)) / timeRequired;
}

double NotesManager::progressByAngle(double progressRate) {
	using namespace std;
	constexpr double EYE_HEIGHT = 1.0;
	constexpr double START_ANGLE = 0.4_pi;//単位はラジアン(0.5以上を設定してはならない)
	constexpr double JUDGE_ANGLE = 0.1_pi;//単位はラジアン
	const static double START_RANGE = EYE_HEIGHT * tan(START_ANGLE);
	const static double JUDGE_RANGE = EYE_HEIGHT * tan(JUDGE_ANGLE);
	double nowRange = START_RANGE - (START_RANGE - JUDGE_RANGE) * progressRate;
	return (START_ANGLE - atan(nowRange / EYE_HEIGHT)) / (START_ANGLE - JUDGE_ANGLE);
}

double NotesManager::getCurrentPosition(int startPos, int endPos, double progressRate) {
	return startPos + (endPos - startPos) * progressRate;
}

double NotesManager::getScale(double currenty) {
	double temp = currenty / (laneJudgeY - 100);//少し早めに縮小率をもとに戻すため引いてみている
	return  temp;
}

void NotesManager::displayNormal(int lane, int time) {
	double progressRate = progressByAngle(getProgress(time));
	double currentY = getCurrentPosition(laneStartY, laneJudgeY, progressRate);
	if (currentY > laneGoalY) {
		plusItr(displayitr[lane]);
		return;
	}
	double currentX = getCurrentPosition(laneStartX[lane], laneJudgeX[lane], progressRate);
	TextureAsset(U"note").scaled(getScale(currentY)).drawAt(currentX, currentY);
}
void NotesManager::displayLong(int lane, int time, int longtime) {
	//描画位置の計算
	//上側
	double progressRateEnd = progressByAngle(getProgress(longtime));
	double currentEndY = getCurrentPosition(laneStartY, laneJudgeY, progressRateEnd);//描画位置Y座標を計算
	if (currentEndY > laneGoalY) {//描画が終了しているなら
		plusItr(displayitr[lane]);
		return;
	}
	double currentEndX = getCurrentPosition(laneStartX[lane], laneJudgeX[lane], progressRateEnd);//描画位置X座標を計算
	if (currentEndY < laneStartY) {//上側がまだ描画位置に到達していないなら
		currentEndX = laneStartX[lane];
		currentEndY = laneStartY;//初期位置へ固定
	}

	//下側
	double progressRateBgn = progressByAngle(getProgress(time));
	double currentBgnY = getCurrentPosition(laneStartY, laneJudgeY, progressRateBgn);//描画位置Y座標を計算
	double currentBgnX = getCurrentPosition(laneStartX[lane], laneJudgeX[lane], progressRateBgn);//描画位置X座標を計算

	//拡大率計算
	double scaleEnd = getScale(currentEndY);
	double scaleBgn = getScale(currentBgnY);

	//描画処理
	for (int linex = 0; linex <= (TextureAsset(U"note").width() / 2); linex++) {
		Line(currentEndX + linex * scaleEnd, currentEndY, currentBgnX + linex * scaleBgn, currentBgnY).draw(1, Color(150 + linex * 2, 50, 50));
		Line(currentEndX - linex * scaleEnd, currentEndY, currentBgnX - linex * scaleBgn, currentBgnY).draw(1, Color(150 + linex * 2, 50, 50));
	}
	
	TextureAsset(U"note").scaled(scaleEnd).drawAt(currentEndX, currentEndY);
	TextureAsset(U"note").scaled(scaleBgn).drawAt(currentBgnX, currentBgnY);
}

/*
void NotesManager::effectInit() {
	enum USE_EFFECT {
		NORMAL,
		PARFECT
	};
	FlipEffect effectNormal(U"resources/images/effect/sol.png", 43, 43, 0, 0);
	FlipEffect effectParfect(U"resources/images/effect/magic.png", 43, 43, 0, 0);
	useFlipEffect.push_back(effectNormal);
	useFlipEffect.push_back(effectParfect);
}
void NotesManager::drawAllEffect() {
	for (auto& flip : useFlipEffect) {
		flip.draw();
	}
}
useFlipEffect[NORMAL].update();.
*/
void NotesManager::setEvent(Massage msg, int val) {
	notessubject->setEvent(msg, val);//イベントオブジェクトセット
	notessubject->notifyObservers();//イベント起動
}

