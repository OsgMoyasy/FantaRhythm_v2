#include "NotesManager.h"


enum class NotesManager::NOTESTYPE {
	NORMAL,
	LONG,
	CRITICAL,
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
	int judgetime;
	bool display;
	float shadow;
};

struct NotesManager::ProPos {
	double scale;
	double x;
	double y;
};

NotesManager::NotesManager(NotesSubject* sub, const String& difpath) {
	TextureAsset::Register(U"note", U"resources/images/items/Nort3rd.png", AssetParameter::LoadAsync());
	TextureAsset::Register(U"longnote", U"resources/images/items/Nort2nd.png", AssetParameter::LoadAsync());
	TextureAsset::Register(U"cri", U"resources/images/items/Nort2.png", AssetParameter::LoadAsync());


  
	TextureAsset::Register(U"judgeline", U"resources/images/items/judgeline.png", AssetParameter::LoadAsync());

	effect.set(JUDGE::GOOD, U"resources/images/effects/nortsEffect.png", 100, 100);
	effect.set(JUDGE::GREAT, U"resources/images/effects/nortsEffect2.png", 100, 100);
	effect[JUDGE::GOOD]->setSE(U"resources/musics/effects/1nort.mp3");
	effect[JUDGE::GREAT]->setSE(U"resources/musics/effects/1nort.mp3");

	//ジャッジエフェクト初期化
	judgeEffect = new JudgeEffect(U"resources/images/items/judgeeffect/");

	comboImNum = new ComboImNumber(Window::Width() - 270, Window::Height() - 230, 200);


	CSVData csv;//譜面の取得　多次元配列で管理 0 判定時間(ms) 1 長さ？ 2 流すレーン[0-3]
	//Print << difpath;

	notessubject = sub;//サブジェクトの登録

	csv.load(difpath);//譜面のロード
	NotesManager::Notes note;
	for (int row = 0; row < csv.rows(); row++) {
		note.time = csv.get<int>(row, 0);
		note.longtime = note.time + csv.get<int>(row, 1);
		int lane = csv.get<int>(row, 2);
		switch (csv.get<int>(row, 3)) {
		case 0:
			if (note.time % 21 == 0) {
				note.type = NOTESTYPE::CRITICAL;
			}
			else {
				note.type = NOTESTYPE::NORMAL;
			}
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
		note.shadow = 0;
		notelist[lane].push_back(note);
	}
	note.type = NOTESTYPE::SENTINEL;//番兵の追加
	note.time = 0;
	note.longtime = 0;
	note.display = false;

	for (int i = 0; i < JUDGE::TYPE::TYPE_SIZE; i++) {
		judgecount.cnt[i] = 0;
	}
	judgecount.maxcombo = 0;

	for (int lane = 0; lane < LANESIZE; lane++) {//レーンごとに到着時間を格納
		notelist[lane].push_back(note);//番兵の設置
		displayitr[lane] = checkitr[lane] = notelist[lane].begin();//チェック用のイテレータ初期
		pressedkey[lane] = 0;
	}

	//描画関係の変数の初期化
	//レーン基準円の位置
	int centerX = Window::Width() / 2;
	int centerY = 0;

	int startRange = 100;			//  上端の基準円の原点からの距離
	int judgeRange = 600;			//判定線の基準円の原点からの距離
	double laneBetween = 0.08_pi;	//レーン間の角度(基準円上の)
	double laneCenter = 1.5_pi;		//レーン群の中心の角度(基準円上の)

	for (int lane = 0;lane < LANESIZE;lane++) {
		double angleInCircle = laneCenter + (lane - (LANESIZE - 1) / 2.0) * laneBetween;	//当該レーンの角度(基準円上の)
		laneStartX[lane] = centerX + (int)(cos(angleInCircle) * startRange);
		laneStartY[lane] = centerY - (int)(sin(angleInCircle) * startRange);

		laneJudgeX[lane] = centerX + (int)(cos(angleInCircle) * judgeRange);
		laneJudgeY[lane] = centerY - (int)(sin(angleInCircle) * judgeRange);
	}
	//Y座標
	laneGoalY = 730;//(画面Y座標限界 + 10)

	//速さ
	notespeed = 1.2;
	timeRequired = 1500 / notespeed;
	//ノーツの大きさ
	notewidth = TextureAsset(U"note").width();
	laneStartScale = 0.2;
	laneJudgeScale = 1.2;

	for (int i = 0; i < LANESIZE; i++) {
		down[i] = 0;
		press[i] = 0;
	}

}
NotesManager::~NotesManager() {
	TextureAsset::Unregister(U"note");
	TextureAsset::Unregister(U"longnote");
	TextureAsset::Unregister(U"cri");
	TextureAsset::Unregister(U"judgeline");
	delete comboImNum;
	delete judgeEffect;
}

bool NotesManager::isReady() {
	if (TextureAsset::IsReady(U"note") &&
		TextureAsset::IsReady(U"longnote")&&
		TextureAsset::IsReady(U"cri")&&
		TextureAsset::IsReady(U"judgeline")) {
		return true;
	}
	return false;
}

void NotesManager::update(void)
{
	nowtime = (int)(MusicManager::getMusicTime());
	checkAttack();
	controlJudge();
	judgeEffect->update();
	comboImNum->update();
}


void NotesManager::plusItr(noteitr& itr) {
	if (itr->type != NOTESTYPE::SENTINEL)//番兵かどうか判定
		itr++;
}


void NotesManager::checkAttack(void) {
	MyKey::getGameDownKey(down);
	MyKey::getGamePressKey(press);
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
	for (int lane = 0;lane < LANESIZE;lane++) {
		switch (checkitr[lane]->type) {
		case NOTESTYPE::NORMAL:
			judgeNormal(lane);
			break;
		case NOTESTYPE::LONG:
			judgeLong(lane);
			break;
		case NOTESTYPE::CRITICAL:
			judgeCritical(lane);
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
		playNotesEffect(getProPos(lane, nowtime), type);
		judgeEvent(type, lane);
		return;
	}
	else if(nowtime > time + JUDGE_RANGE::BAD){
		judgeEvent(JUDGE::BAD, lane);
		return;
	}
}
void NotesManager::judgeLong(int lane) {
	if (pressedkey[lane] == false && down[lane]) {
		JUDGE::TYPE type = NoteisHit(checkitr[lane]->time);
		if (type <= JUDGE::GOOD) {//下端で押されたら(この判定はJUDGEenumの並び順に依存している)
			playNotesEffect(getProPos(lane, nowtime), type);
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
			playNotesEffect(getProPos(lane, nowtime), type);
			if (type == JUDGE::TYPE::NONE) {//早すぎたときnoneになるのでbad格納
				type = JUDGE::TYPE::BAD;
			}
			return judgeLongEvent(type, lane);
		}
	}
	
	if (nowtime >= checkitr[lane]->time + JUDGE_RANGE::GOOD) {//判定を超えた時
		return judgeLongEvent(JUDGE::BAD, lane);
	}
}
void NotesManager::judgeCritical(int lane) {
	static int prevTime[LANESIZE] = {0, 0, 0, 0};	//押され始めの時間保持
	static int pressHold[LANESIZE] = {0, 0, 0, 0};	//押され始めのボタン状態保持(上だけなのか下だけなのか両方なのか）
	static JUDGE::TYPE typeHold[LANESIZE] = { JUDGE::NONE,JUDGE::NONE,JUDGE::NONE,JUDGE::NONE };//押され始めの判定を保持
	//judgeCriticalEventを呼び,上記のローカル静的変数を初期化するマクロ
	#define JUDGE_CRITICAL_EVENT  judgeCriticalEvent(typeHold[lane], lane, pressHold[lane]);\
								  pressHold[lane] = 0; prevTime[lane] = 0;\
								  typeHold[lane] = JUDGE::NONE;

	JUDGE::TYPE type = NoteisHit(checkitr[lane]->time);
	if (down[lane] ){//ボタンが押され始めかどうかを判定
		if (type < JUDGE::NONE) {//判定可能なら
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
			playNotesEffect(getProPos(lane, nowtime), typeHold[lane]);
			JUDGE_CRITICAL_EVENT;//同時押しイベント
			return;
		}
		else if (press[lane] == 0 ||				//ボタンが途中で離されるか
			nowtime - prevTime[lane] > 50) {	//同時押しされてない場合の処理
			playNotesEffect(getProPos(lane, nowtime), typeHold[lane]);
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
	judgecount.cnt[type]++;//判定をカウントアップ
	judgeEffect->setEffect(type);//判定エフェクトセット
	if(type == JUDGE::BAD){
		checkitr[lane]->shadow = 0.1;
		combo.reset();
		comboImNum->resetCombo();
		setEvent(Massage::DAMAGE, lane);
	}
	else {
		if (next) { checkitr[lane]->display = false; }//ディスプレイ表示オフ
		combo.add();
		judgecount.maxcombo = combo.getMax();
		comboImNum->setCombo(combo.get());
		switch (down[lane]) {//成功イベント送信
		case PSHBTN::UP:
			setEvent(Massage::UPATTACK, lane);
			break;
		case PSHBTN::DOWN:
			setEvent(Massage::DOWNATTACK, lane);
			break;
		}
	}

	if (next) {
		noteNext(lane);
	}
}

void NotesManager::judgeCriticalEvent(JUDGE::TYPE type, int lane, int buttonType) {
	judgecount.cnt[type]++;//判定をカウントアップ
	judgeEffect->setEffect(type);//判定エフェクトセット
	if (type == JUDGE::BAD) {//BADイベント送信
		checkitr[lane]->shadow = 0.1;
		setEvent(Massage::CRITICALDAMAGE, lane);
	}
	else {
		checkitr[lane]->display = false;//ディスプレイ表示オフ
		switch (buttonType) {//成功イベント送信
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
	noteNext(lane);
}
void NotesManager::noteNext(int lane) {
	plusItr(checkitr[lane]);//判定対象を次に進める
}


JUDGE::JudgeCount* NotesManager::getJudgeCount() {
	return &judgecount;
}


void NotesManager::draw(void){

	//デバッグ用
	/*
	for (int i = 0; i < 4; i++) {
		Print << judgecount.cnt[i];
	}
	*/
	//Print << U"NotesCombo=" << combo.get();

	for(int lane = 0; lane < LANESIZE; lane++){
		displayLane(lane);
		displayButton(lane);

		for (noteitr itr = displayitr[lane]; itr != notelist[lane].end(); itr++) {
			if (nowtime < itr->time - timeRequired)//描画前なら描画打ち切り
				break;
			if (itr->display == false)
				continue;

			switch (itr->type){
			case NOTESTYPE::NORMAL:
				displayNormal(lane, itr->time, itr->shadow);
				break;
			case NOTESTYPE::LONG:
				displayLong(lane, itr->time, itr->longtime, itr->shadow);
				break;
			case NOTESTYPE::CRITICAL:
				displayCritical(lane, itr->time, itr->shadow);
				break;
			default:
				break;
			}

			if (0 < itr->shadow && itr->shadow < 0.5) {
				itr->shadow += 0.15;
			}
		}	
	}

	effect.draw();//再生中の全てのエフェクトを描画
	judgeEffect->draw();//判定エフェクト描画
	comboImNum->draw();//コンボエフェクト描画
}


void NotesManager::displayLane(int lane) {
	struct line{
		Vec2 pos1;
		Vec2 pos2;
		double progressRate;
	};
	static Array<line> laneRoad[LANESIZE];

	static bool first = true;
	if (first) {
		constexpr int LINEBETWEEN = 3;	//線の間隔[ms]

		for (int lane = 0;lane < LANESIZE;lane++) {
			for (int i = nowtime;i < nowtime + timeRequired;i += LINEBETWEEN) {
				double progressRate = progressByAngle(getProgress(i));
				double currentY = getCurrentPosition(laneStartY[lane], laneJudgeY[lane], progressRate);
				double currentX = getCurrentPosition(laneStartX[lane], laneJudgeX[lane], progressRate);
				double nowNoteRadius = notewidth / 2 * getCurrentPosition(laneStartScale, laneJudgeScale, progressRate);

				laneRoad[lane].push_back({
					{ currentX - nowNoteRadius, currentY},
					{ currentX + nowNoteRadius, currentY},
					progressRate
					});
			}
		}
		first = false;
	}

	line& prev = laneRoad[lane][0];
	for (auto& line : laneRoad[lane]) {
		double weight = line.progressRate * 4;	//線の太さ
		ColorF color = ColorF(Palette::Blue, line.progressRate / 2);	//線の色と透明度
		Line(line.pos1, line.pos2).draw(weight, color);

//		ColorF color2 = ColorF(Palette::Blue, line.progressRate);	//線の色と透明度
//		Line(line.pos1, prev.pos1).draw(weight * 2 / 3, color2);
//		Line(line.pos2, prev.pos2).draw(weight * 2 / 3, color2);
//		prev = line;
	}
}
void NotesManager::displayButton(int lane) {
	Color color = press[lane] ? Palette::Skyblue : Palette::White;	//押されているボタンに対応した円を変色
	static double circleScale = notewidth / 2 * (double)laneJudgeScale;
	Circle(laneJudgeX[lane], laneJudgeY[lane], circleScale).draw(color);
	Circle(laneJudgeX[lane], laneJudgeY[lane], circleScale).drawFrame(3, Palette::Blue);
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
	double currentY = getCurrentPosition(laneStartY[lane], laneJudgeY[lane], progressRate);
	double currentX = getCurrentPosition(laneStartX[lane], laneJudgeX[lane], progressRate);
	double scale = getCurrentPosition(laneStartScale, laneJudgeScale, progressRate);
	return { scale ,currentX ,currentY };
}


void NotesManager::displayNormal(int lane, int time, float shadow) {
	ProPos now = getProPos(lane, time);
	if (now.y > laneGoalY) {
		plusItr(displayitr[lane]);
		return;
	}
	TextureAsset(U"note").scaled(now.scale).drawAt(now.x, now.y);
	Circle(now.x, now.y, now.scale * notewidth / 2).draw(ColorF(Palette::Black, shadow));
}
void NotesManager::displayLong(int lane, int time, int longtime, float shadow) {
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

	Color lineColor;	//伸ばし棒　色指定

	for (int i = longt - BETW;i > time;i -= BETW) {
		a = b;
		b = getProPos(lane, i);
		for (int linex = 0; linex <= (TextureAsset(U"note").width() / 2); linex++) {
			lineColor = longLineColor(linex, lane, time);
			Line(a.x + linex * a.scale, a.y, b.x + linex * b.scale, b.y).draw(1, lineColor);
			Line(a.x - linex * a.scale, a.y, b.x - linex * b.scale, b.y).draw(1, lineColor);
			Line(a.x + linex * a.scale, a.y, b.x + linex * b.scale, b.y).draw(1, ColorF(Palette::Black, shadow));
			Line(a.x - linex * a.scale, a.y, b.x - linex * b.scale, b.y).draw(1, ColorF(Palette::Black, shadow));
		}
	}
	for (int linex = 0; linex <= (TextureAsset(U"note").width() / 2); linex++) {
		lineColor = longLineColor(linex, lane, time);
		Line(b.x + linex * b.scale, b.y, bgn.x + linex * bgn.scale, bgn.y).draw(1, lineColor);
		Line(b.x - linex * b.scale, b.y, bgn.x - linex * bgn.scale, bgn.y).draw(1, lineColor);
		Line(b.x + linex * b.scale, b.y, bgn.x + linex * bgn.scale, bgn.y).draw(1, ColorF(Palette::Black, shadow));
		Line(b.x - linex * b.scale, b.y, bgn.x - linex * bgn.scale, bgn.y).draw(1, ColorF(Palette::Black, shadow));
	}

	TextureAsset(U"longnote").scaled(end.scale).drawAt(end.x, end.y);
	TextureAsset(U"longnote").scaled(bgn.scale).drawAt(bgn.x, bgn.y);
	Circle(end.x, end.y, end.scale * notewidth / 2).draw(ColorF(Palette::Black, shadow));
	Circle(bgn.x, bgn.y, bgn.scale * notewidth / 2).draw(ColorF(Palette::Black, shadow));
}

Color NotesManager::longLineColor(int linex, int lane, int time) {
	if (pressedkey[lane] > 0 && (*checkitr[lane]).time == time) {
		return Color(200 + linex * 2.5, 50, 50);
	}
	else {
		return Color(150 + linex * 2.0, 50, 50);
	}
}

void NotesManager::displayCritical(int lane, int time, float shadow) {
	ProPos now = getProPos(lane, time);
	if (now.y > laneGoalY) {
		plusItr(displayitr[lane]);
		return;
	}
	TextureAsset(U"cri").scaled(now.scale).drawAt(now.x, now.y);
	Circle(now.x, now.y, now.scale * notewidth / 2).draw(ColorF(Palette::Black, shadow));
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

NotesManager::Combo::Combo() {
	count = 0;
	max = 0;
}
void NotesManager::Combo::add(int val) {
	count += val;
	max = max > count ? max : count;
}
void NotesManager::Combo::reset() {
	count = 0;
}
int NotesManager::Combo::get() {
	return count;
}
int NotesManager::Combo::getMax() {
	return max;
}
