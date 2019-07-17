#include "NotesManager.h"



NotesManager::NotesManager(const String& difpath) {
	TextureAsset::Register(U"note", U"resources/images/items/Nort3rd.png");
	TextureAsset::Preload(U"note");
	TextureAsset::Register(U"longef", U"resources/images/items/longNortsEffect2.png");
	TextureAsset::Preload(U"longef");
	CSVData csv;//譜面の取得　多次元配列で管理 0 判定時間(ms) 1 長さ？ 2 流すレーン[0-3]
	Print << difpath;

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
	for (int i = 0; i < LANESIZE; i++) {//レーンごとに到着時間を格納
		displayitr[i] = checkitr[i] = notelist[i].begin();//チェック用のイテレータ初期
		longflag[i] = false;
	}

	laneStartX[0] = 520;
	laneStartX[1] = 600;
	laneStartX[2] = 680;
	laneStartX[3] = 760;
	laneJudgeX[0] = 430;
	laneJudgeX[1] = 570;
	laneJudgeX[2] = 710;
	laneJudgeX[3] = 850;
	laneStartY = 100;
	laneJudgeY = 650;//CHECKLINEYの代わり
	laneGoalY = 730;//画面Y座標限界+10
	notespeed = 1.5;
	timeRequired = 2000 / notespeed;//ノーツの出現から判定まで流れる時間[ms]

}
NotesManager::~NotesManager() {
	TextureAsset::UnregisterAll();
}


void NotesManager::update(void)
{
	nowTime = MusicManager::getMusicTime();
	checkAttack();
	controlJudge();
}

void NotesManager::plusItr(int lane, std::list<Notes>::iterator& itr) {
	if (itr != notelist[lane].end())
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
	if (down[lane] && checktime <= bad) {
		if (checktime <= great) {
			//GREAT
		}
		else if (checktime < good) {
			//GOOD
		}
		else{

		}
		checkitr[lane]->display = false;
		plusItr(lane, checkitr[lane]);
	}
	else if (nowTime >= checkitr[lane]->time + bad) {
		plusItr(lane, checkitr[lane]);
	}
}
void NotesManager::judgeLong(int lane) {
	static const int good = 150;//判定の最大範囲
	int checktime = abs(nowTime - checkitr[lane]->time);

	//もう少しゲームの内容すり合わせ
	//スコアの出方　hpの減るタイミング
	//スレッドから取得する値
	//ロングノーツの判定
	//ロングノーツ流れ方	//現在の欠点　押しすぎ時とか
	/*	条件整理　
		ボタン押下			→					判定外か判断
			↓										↓
		判定位置通り過ぎたら止める					↓
		ボタン離した　　　　→　離していない		↓	
			↓						↓				↓
		スコア処理				判定外判断（good内)	↓
			↓										↓
		イテレータ進める←←←←←←←←←←←←←
	
	*/
	if (down[lane] && checktime <= good) {//押されたらフラグを立てる
		longflag[lane] = true;
	}
	if (longflag[lane]) {
		if (press[lane]) {//ボタン押下中
			checkitr[lane]->time = (int)(nowTime);//判定位置以降で止める
			if (nowTime >= checkitr[lane]->longtime) {//押されているまま判定位置へ来た時
				checkitr[lane]->time = checkitr[lane]->longtime;
				if (nowTime >= checkitr[lane]->longtime + good) {//押されている間は判定時間ぎりぎりまで待機させるためgood加算
					longflag[lane] = false;
					checkitr[lane]++;
				}
			}
		}
		else {//離した
			if ((abs(nowTime - checkitr[lane]->longtime)) <= good) {
				checkitr[lane]->display = false;//成功
			}
			checkitr[lane]++;
			longflag[lane] = false;
		}
	}
	else if (nowTime >= checkitr[lane]->longtime) {//押されていないまま終了時
		checkitr[lane]++;
		longflag[lane] = false;
	}
}



void NotesManager::draw(void){
	Line(0, laneJudgeY, 1920, laneJudgeY).draw(3, Palette::Black);
	for(int i = 0; i < LANESIZE; i++){
		Line(laneStartX[i], laneStartY, laneJudgeX[i], laneJudgeY).draw(1, Palette::Red);
		for (std::list<Notes>::iterator itr = displayitr[i]; itr != notelist[i].end(); itr++) {
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
int NotesManager::getCurrentPosition(int startPos, int endPos, double progressRate) {
	return (int)(startPos + (endPos - startPos) * progressRate);
}

void NotesManager::displayNormal(int lane, int time) {
	double progressRate = getProgress(time);
	double currentY = laneStartY + (laneJudgeY - laneStartY) * progressRate;
	if (currentY > laneGoalY) {
		plusItr(lane ,displayitr[lane]);
		return;
	}
	double currentX = laneStartX[lane] + (laneJudgeX[lane] - laneStartX[lane]) * progressRate;
	TextureAsset(U"note").drawAt(currentX, currentY);
}
void NotesManager::displayLong(int lane, int time, int longtime) {
	double progressRateEnd = getProgress(longtime);
	double currentEndY = laneStartY + (laneJudgeY - laneStartY) * progressRateEnd;
	if (currentEndY > laneGoalY) {
		plusItr(lane, displayitr[lane]);
		return;
	}
	double currentEndX = laneStartX[lane] + (laneJudgeX[lane] - laneStartX[lane]) * progressRateEnd;

	double progressRateBgn = getProgress(time);
	double currentBgnY = laneStartY + (laneJudgeY - laneStartY) * progressRateBgn;
	double currentBgnX = laneStartX[lane] + (laneJudgeX[lane] - laneStartX[lane]) * progressRateBgn;

	for (int linex = 0; linex <= 25; linex++) {
		Line(currentEndX + linex, currentEndY, currentBgnX + linex, currentBgnY).draw(1, Color(150 + linex * 2, 50, 50));
		Line(currentEndX - linex, currentEndY, currentBgnX - linex, currentBgnY).draw(1, Color(150 + linex * 2, 50, 50));
	}
	TextureAsset(U"note").drawAt(currentEndX, currentEndY);
	TextureAsset(U"note").drawAt(currentBgnX, currentBgnY);
}
