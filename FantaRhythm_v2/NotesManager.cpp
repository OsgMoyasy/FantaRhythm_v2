#include "NotesManager.h"


NotesManager::NotesManager(NotesSubject* sub, const String& difpath) {
	TextureAsset::Register(U"note", U"resources/images/items/Nort3rd.png");
	TextureAsset::Preload(U"note");
	TextureAsset::Register(U"longef", U"resources/images/items/longNortsEffect2.png");
	TextureAsset::Preload(U"longef");


	CSVData csv;//譜面の取得　多次元配列で管理 0 判定時間(ms) 1 長さ？ 2 流すレーン[0-3]
	Print << difpath;

	subject = sub;

	csv.load(difpath);//譜面のロード
	NotesManager::Notes note;
	for (int j = 0; j < csv.rows(); j++) {
		int lane = csv.get<int>(j, 2);
		note.time = csv.get<int>(j, 0);
		note.longtime = note.time + csv.get<int>(j, 1);
		switch (csv.get<int>(j, 3)) {
			case 0:
				note.type = NORMAL;
				break;
			case 1:
				note.type = LONG;
				break;
		}
		note.display = true;
		notelist[lane].push_back(note);
	}

	note.type = SENTINEL;
	note.time = 0;
	note.longtime = 0;
	note.display = false;

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
	nowTime = (int)(MusicManager::getMusicTime());
	checkAttack();
	controlJudge();
}

void NotesManager::plusItr(noteitr& itr) {
	if (itr->type != SENTINEL)//番兵かどうか判定
		itr++;
}


void NotesManager::checkAttack(void) {
	 down[0] = KeyA.down();
	press[0] = KeyA.pressed();
	 down[1] = KeyS.down();
	press[1] = KeyS.pressed();
	 down[2] = KeyD.down();
	press[2] = KeyD.pressed();
	 down[3] = KeyF.down();
	press[3] = KeyF.pressed();
}
void NotesManager::controlJudge(void) {
	for (int i = 0;i < LANESIZE;i++) {
		switch (checkitr[i]->type) {
		case NORMAL:
			judgeNormal(i);
			break;
		case LONG:
			judgeLong(i);
			break;
		default:
			break;
		}
	}
}
void NotesManager::judgeNormal(int lane) {
	static const int bad = 200;//判定の最大範囲[ms]÷2
	static const int good = 100;//GOOD判定範囲[ms]÷2
	static const int great = 25;//GREAT判定範囲[ms]÷2
	int checktime = abs(nowTime - checkitr[lane]->time);
	if (down[lane] && checktime <= bad) {//押されてるかつ判定時間内なら判定処理
		if (checktime <= great) {//GREAT
			setEvent(Massage::UPATTACK, lane);
		}
		else if (checktime < good) {//GOOD
			setEvent(Massage::UPATTACK, lane);
		}
		else{//BAD
			setEvent(Massage::SMALLDAMAGE, lane);
		}
		checkitr[lane]->display = false;
		plusItr(checkitr[lane]);
	}
	else if (nowTime >= checkitr[lane]->time + bad) {//押されてないまま終了時
		setEvent(Massage::SMALLDAMAGE, lane);
		plusItr(checkitr[lane]);
	}
}
void NotesManager::judgeLong(int lane) {
	static const int good = 150;//判定の最大範囲
	int checktime = abs(nowTime - checkitr[lane]->time);

	if (down[lane] && checktime <= good) {//押されたらフラグを立てる
		longflag[lane] = true;
	}
	if (longflag[lane]) {
		if (press[lane]) {//ボタン押下中
			checkitr[lane]->time = (int)(nowTime);//判定位置以降で止める
			if (nowTime >= checkitr[lane]->longtime) {//押されているまま判定位置へ来た時
				checkitr[lane]->time = checkitr[lane]->longtime;
				if (nowTime >= checkitr[lane]->longtime + good) {//押されている間の判定時間も過ぎたとき
					setEvent(Massage::SMALLDAMAGE, lane);
					longflag[lane] = false;
					plusItr(checkitr[lane]);
				}
			}
		}
		else {//離した
			if ((abs(nowTime - checkitr[lane]->longtime)) <= good) {
				setEvent(Massage::UPATTACK, lane);
				checkitr[lane]->display = false;//成功
			}
			else {//失敗
				setEvent(Massage::SMALLDAMAGE, lane);
			}
			plusItr(checkitr[lane]);
			longflag[lane] = false;
		}
	}
	else if (nowTime >= checkitr[lane]->longtime) {//押されていないまま終了時
		setEvent(Massage::SMALLDAMAGE, lane);
		plusItr(checkitr[lane]);
		longflag[lane] = false;
	}
}



void NotesManager::draw(void){
	Line(0, laneJudgeY, 1920, laneJudgeY).draw(3, Palette::Black);
	for(int i = 0; i < LANESIZE; i++){
		Line(laneStartX[i], laneStartY, laneJudgeX[i], laneJudgeY).draw(1, Palette::Red);
		for (noteitr itr = displayitr[i]; itr != notelist[i].end(); itr++) {
			if (nowTime < itr->time - timeRequired)//描画前なら描画打ち切り
				break;
			if (itr->display == false)
				continue;

			switch (itr->type){
			case NORMAL:
				displayNormal(i, itr->time);
				break;
			case LONG:
				displayLong(i, itr->time, itr->longtime);
				break;
			default:
				break;
			}
		}	
	}
}

double NotesManager::getProgress(int time) {
	return (timeRequired - (time - nowTime)) / timeRequired;
}

double NotesManager::progressByAngle(double progressRate) {
	using namespace std;
	constexpr double PI = 3.1415;
	constexpr double EYE_HEIGHT = 1.0;
	constexpr double START_ANGLE = 0.4 * PI;//単位はラジアン(0.5以上を設定してはならない)
	constexpr double JUDGE_ANGLE = 0.1 * PI;//単位はラジアン
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
	for (int linex = 0; linex <= 25; linex++) {
		Line(currentEndX + linex * scaleEnd, currentEndY, currentBgnX + linex * scaleBgn, currentBgnY).draw(1, Color(150 + linex * 2, 50, 50));
		Line(currentEndX - linex * scaleEnd, currentEndY, currentBgnX - linex * scaleBgn, currentBgnY).draw(1, Color(150 + linex * 2, 50, 50));
	}
	
	TextureAsset(U"note").scaled(scaleEnd).drawAt(currentEndX, currentEndY);
	TextureAsset(U"note").scaled(scaleBgn).drawAt(currentBgnX, currentBgnY);
}

void NotesManager::setEvent(Massage msg, int val) {
	subject->setEvent(msg, val);//イベントオブジェクトセット
	subject->notifyObservers();//イベント起動
}
