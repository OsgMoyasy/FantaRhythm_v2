#include "NotesManager.h"


enum class NotesManager::NOTESTYPE {
	NORMAL,
	LONG,
	CRITICAL,
	SENTINEL,
};

namespace PSHBTN {
	constexpr int NONE = 0;
	constexpr int UP = 1;
	constexpr int DOWN = 2;
	constexpr int BOTH = 3;
}

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
	int judgetime;
	bool display;
};

struct NotesManager::ProPos {
	double scale;
	double x;
	double y;
};

NotesManager::NotesManager(NotesSubject* sub, const String& difpath) {
	TextureAsset::Register(U"note", U"resources/images/items/Nort3rd.png");
	TextureAsset::Preload(U"note");
	TextureAsset::Register(U"cri", U"resources/images/items/Nort2nd.png");
	TextureAsset::Preload(U"cri");

	effect.set(JUDGE::GOOD, U"resources/images/effect/nortsEffect.png", 100, 100);
	effect.set(JUDGE::GREAT, U"resources/images/effect/nortsEffect2.png", 100, 100);

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
			note.judgetime = note.time;
			break;
		case 1:
			note.type = NOTESTYPE::LONG;
			note.judgetime = note.longtime;
			break;
		case 2:
			note.type = NOTESTYPE::CRITICAL;
			note.judgetime = note.time;
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
		pressedkey[lane] = 0;
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
	//ノーツの大きさ
	notewidth = TextureAsset(U"note").width();
	laneStartScale = 0.2;
	laneJudgeScale = 1.0;

	for (int i = 0; i < LANESIZE; i++) {
		down[i] = 0;
		press[i] = 0;
	}
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
	down[0] = KeyQ.down() ? PSHBTN::UP : 0;
	press[0] = KeyQ.pressed() ? PSHBTN::UP : 0;

	down[1] = KeyW.down() ? PSHBTN::UP : 0;
	press[1] = KeyW.pressed() ? PSHBTN::UP : 0;

	down[2] = KeyE.down() ? PSHBTN::UP : 0;
	press[2] = KeyE.pressed() ? PSHBTN::UP : 0;

	down[3] = KeyR.down() ? PSHBTN::UP : 0;
	press[3] = KeyR.pressed() ? PSHBTN::UP : 0;


	down[0] += KeyA.down() ? PSHBTN::DOWN : 0;
	press[0] += KeyA.pressed() ? PSHBTN::DOWN : 0;

	down[1] += KeyS.down() ? PSHBTN::DOWN : 0;
	press[1] += KeyS.pressed() ? PSHBTN::DOWN : 0;

	down[2] += KeyD.down() ? PSHBTN::DOWN : 0;
	press[2] += KeyD.pressed() ? PSHBTN::DOWN : 0;

	down[3] += KeyF.down() ? PSHBTN::DOWN : 0;
	press[3] += KeyF.pressed() ? PSHBTN::DOWN : 0;
}
JUDGE::TYPE NotesManager::NoteisHit(int judgetime) {//判定するタイミングからJUDGEのタイプを返す
	return judgeType(abs(nowtime - judgetime));
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


void NotesManager::controlJudge(void) {
	for (int i = 0;i < LANESIZE;i++) {
		switch (checkitr[i]->type) {
		case NOTESTYPE::NORMAL:
			judgeNormal(i);
			break;
		case NOTESTYPE::LONG:
			judgeLong(i);
			break;
		case NOTESTYPE::CRITICAL:
			judgeCritical(i);
			break;
		default:
			break;
		}
	}
}
void NotesManager::judgeNormal(int lane) {
	int time = checkitr[lane]->time;
	JUDGE::TYPE type = NoteisHit(time);
	if (down[lane] && type != JUDGE::NONE) {//判定時間過ぎるか判定可能でボタンが押されている時
		return judgeEvent(type, lane);
	}
	else if(nowtime > time + JUDGE_RANGE::BAD){
		return judgeEvent(JUDGE::BAD, lane);
	}
}
void NotesManager::judgeLong(int lane) {
	if (pressedkey[lane] == false && down[lane]) {
		JUDGE::TYPE type = NoteisHit(checkitr[lane]->time);
		if (type <= JUDGE::GOOD) {//下端で押されたら(この判定はJUDGEenumの並び順に依存している)
			pressedkey[lane] = down[lane];//そのロングノーツの判定を有効化
			return judgeEvent(type, lane, false);
		}
	}

	if (pressedkey[lane] > 0) {//離すときの処理
		if ((press[lane] & pressedkey[lane]) == pressedkey[lane]) {//ボタン押下中
			checkitr[lane]->time = (int)(nowtime);//判定位置以降で下側を止める
			if (nowtime >= checkitr[lane]->judgetime) {//押されているままノーツの上端を過ぎた時
				checkitr[lane]->longtime = (int)(nowtime);//判定位置以降で上側を止める
				if (nowtime >= checkitr[lane]->judgetime + JUDGE_RANGE::GOOD) {//過ぎたときの判定も取るため
					return judgeLongEvent(JUDGE::BAD, lane);//強制的に次のノーツへ処理を移行
				}
				
			}
		}
		else {//離した
			JUDGE::TYPE type = NoteisHit(checkitr[lane]->judgetime);
			return judgeLongEvent(type, lane);
		}
	}
	
	if (nowtime >= checkitr[lane]->judgetime + JUDGE_RANGE::GOOD) {//判定を超えた時
		return judgeLongEvent(JUDGE::BAD, lane);
	}
}
void NotesManager::judgeCritical(int lane) {
	static int prevTime[LANESIZE]{ 0, 0, 0, 0 };
	static int pressHold[LANESIZE] = { 0,0,0,0 };
	static JUDGE::TYPE typeHold[LANESIZE] = { JUDGE::NONE,JUDGE::NONE,JUDGE::NONE,JUDGE::NONE };
	//judgeCriticalEventを呼び上記のローカル静的変数を初期化するマクロ
	#define JUDGE_CRITICAL_EVENT  judgeCriticalEvent(typeHold[lane], lane, pressHold[lane]);\
								  pressHold[lane] = 0;prevTime[lane] = 0;\
								  typeHold[lane] = JUDGE::NONE;

	JUDGE::TYPE type = NoteisHit(checkitr[lane]->time);
	if (down[lane] ){//ボタンが押され始めかどうかを判定
		if (type < JUDGE::NONE) {
			if (typeHold[lane] == JUDGE::NONE) {
				pressHold[lane] = down[lane];
				typeHold[lane] = type;
				prevTime[lane] = nowtime;
			}
		}
	}

	//ボタンが押された後の時
	if (pressHold[lane] > 0) {
		if (press[lane] == PSHBTN::BOTH) {//同時押しの場合
			pressHold[lane] = PSHBTN::BOTH;
			JUDGE_CRITICAL_EVENT;//同時押しイベント
			return;
		}
		else if (press[lane] == 0 ||				//ボタンが途中で離されるか
				 nowtime - prevTime[lane] > 50) {	//同時押しされてない場合の処理
			JUDGE_CRITICAL_EVENT;//最初に押した時点のイベントを起こす
			return;
		}
	}
	else {
		if (nowtime > checkitr[lane]->time + JUDGE_RANGE::BAD ||//押さずに時間切れ
			typeHold[lane] == JUDGE::BAD) {//BAD判定
			typeHold[lane] = JUDGE::BAD;
			JUDGE_CRITICAL_EVENT;//BADイベント
			return;
		}
	}
}


void NotesManager::judgeLongEvent(JUDGE::TYPE type, int lane) {
	down[lane] = pressedkey[lane];
	judgeEvent(type, lane);
	pressedkey[lane] = 0;//判定したので長押しの状態を初期化
}
void NotesManager::judgeEvent(JUDGE::TYPE type, int lane, bool next) {
	if (next) {
		noteNext(lane);
	}
	judgecount.cnt[type]++;//判定をカウントアップ
	if(type == JUDGE::BAD){
		setEvent(Massage::DAMAGE, lane);
	}
	else {
		switch (down[lane]) {
		case PSHBTN::UP:
			setEvent(Massage::UPATTACK, lane);
			break;
		case PSHBTN::DOWN:
			setEvent(Massage::DOWNATTACK, lane);
			break;
		}
	}
}
void NotesManager::judgeCriticalEvent(JUDGE::TYPE type, int lane, int buttonType) {
	noteNext(lane);
	judgecount.cnt[type]++;//判定をカウントアップ
	if (type == JUDGE::BAD) {
		setEvent(Massage::CRITICALDAMAGE, lane);
	}
	else {
		switch (buttonType) {
		case PSHBTN::UP:
			setEvent(Massage::UPATTACK, lane);
			setEvent(Massage::CRITICALDAMAGE, lane);
			break;
		case PSHBTN::DOWN:
			setEvent(Massage::DOWNATTACK, lane);
			setEvent(Massage::CRITICALDAMAGE, lane);
			break;
		case PSHBTN::BOTH:
			setEvent(Massage::BOTHATTACK, lane);
			break;
		}
	}
}
void NotesManager::noteNext(int lane) {
	checkitr[lane]->display = false;//ディスプレイ表示オフ
	plusItr(checkitr[lane]);//判定対象を次に進める
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
			case NOTESTYPE::CRITICAL:
				displayCritical(i, itr->time);
				break;
			default:
				break;
			}
		}	
	}
	effect.draw();//再生中の全てのエフェクトを描画
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
double NotesManager::getCurrentPosition(double startPos, double endPos, double progressRate) {
	return startPos + (endPos - startPos) * progressRate;
}
NotesManager::ProPos NotesManager::getProPos(int lane, int time) {
	double progressRate = progressByAngle(getProgress(time));
	double currentY = getCurrentPosition(laneStartY, laneJudgeY, progressRate);
	double currentX = getCurrentPosition(laneStartX[lane], laneJudgeX[lane], progressRate);
	double scale = getCurrentPosition(laneStartScale, laneJudgeScale, progressRate);
	return { scale ,currentX ,currentY };
}


void NotesManager::displayNormal(int lane, int time) {
	ProPos now = getProPos(lane, time);
	if (now.y > laneGoalY) {
		plusItr(displayitr[lane]);
		return;
	}
	TextureAsset(U"note").scaled(now.scale).drawAt(now.x, now.y);
}
void NotesManager::displayLong(int lane, int time, int longtime) {
	//描画位置の計算
	//上側
	int longt = longtime < nowtime + timeRequired ? longtime : nowtime + timeRequired;
	ProPos end = getProPos(lane, longt);
	if (end.y > laneGoalY) {
		plusItr(displayitr[lane]);
		return;
	}
	//下側
	ProPos bgn = getProPos(lane, time);

	//描画処理
	ProPos a, b = end;
	constexpr int BETW = 50;//伸ばし棒の太さの再計算の間隔[ms]
	for (int i = longt - BETW;i > time;i -= BETW) {
		a = b;
		b = getProPos(lane, i);
		for (int linex = 0; linex <= (TextureAsset(U"note").width() / 2); linex++) {
			Line(a.x + linex * a.scale, a.y, b.x + linex * b.scale, b.y).draw(1, Color(150 + linex * 2, 50, 50));
			Line(a.x - linex * a.scale, a.y, b.x - linex * b.scale, b.y).draw(1, Color(150 + linex * 2, 50, 50));
		}
	}
	for (int linex = 0; linex <= (TextureAsset(U"note").width() / 2); linex++) {
		Line(b.x + linex * b.scale, b.y, bgn.x + linex * bgn.scale, bgn.y).draw(1, Color(150 + linex * 2, 50, 50));
		Line(b.x - linex * b.scale, b.y, bgn.x - linex * bgn.scale, bgn.y).draw(1, Color(150 + linex * 2, 50, 50));
	}

	TextureAsset(U"note").scaled(end.scale).drawAt(end.x, end.y);
	TextureAsset(U"note").scaled(bgn.scale).drawAt(bgn.x, bgn.y);
}
void NotesManager::displayCritical(int lane, int time) {
	ProPos now = getProPos(lane, time);
	if (now.y > laneGoalY) {
		plusItr(displayitr[lane]);
		return;
	}
	TextureAsset(U"cri").scaled(now.scale).drawAt(now.x, now.y);
}
void NotesManager::playNotesEffect(ProPos pos, JUDGE::TYPE type) {
	if (type <= JUDGE::GREAT) {
		effect[JUDGE::GREAT]->play(pos.x, pos.y);
		return;
	}
	if (type <= JUDGE::GOOD) {
		effect[JUDGE::GOOD]->play(pos.x, pos.y);
		return;
	}
}


void NotesManager::setEvent(Massage msg, int val) {
	notessubject->setEvent(msg, val);//イベントオブジェクトセット
	notessubject->notifyObservers();//イベント起動
}

