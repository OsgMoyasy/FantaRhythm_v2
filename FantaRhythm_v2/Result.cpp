#include "Result.h"
#include <vector>
constexpr double NUMBER_SWTIME = 0.1;
constexpr double NUM_ENDTIME = NUMBER_SWTIME * 9 * 1000;
constexpr int NUMIM_WIDTH = 100, NUMIM_HEIGHT = 150;
constexpr int SCOREX =300, SCOREY = 200;

Result::Result(JUDGE::JudgeCount judgeCnt, int totalDamage, bool clearFlag) {
	this->judgeCnt = judgeCnt;
	this->totalDamage = totalDamage;
	this->clearFlag = clearFlag;
	stopwatch.start();
	framecnt = 0;
	alphaBack = 0;
	alphaFont = 0;

	FontAsset::Register(U"font", 50);
	FontAsset::Preload(U"font");
	FontAsset::Register(U"subfont", 30);
	FontAsset::Preload(U"subfont");
	if (!clearFlag) {//ゲームクリア
		//テクスチャ初期化
		TextureAsset::Register(U"back", U"resources/images/back/BackScreen.jpg");
		imNumberInit();
		//効果音初期化
		se = new SE(U"resources/musics/effects/Congratulations.wav");
		se->play();

		stateUpdate = &Result::successUpdate;
		stateDraw = &Result::successDraw;
		int score = calcScore(this->judgeCnt);
		scoreStr = Format(score);
		scoreDraw = U"               ";
	}
	else {//ゲームオーバー
		TextureAsset::Register(U"back", U"resources/images/back/gameOver.jpg");
		stateUpdate = &Result::failedUpdate;
		stateDraw = &Result::failedDraw;
	}
	TextureAsset::Preload(U"back");//背景のロード
}
Result::~Result(void) {
	FontAsset::Unregister(U"font");
	FontAsset::Unregister(U"subfont");
	TextureAsset::UnregisterAll();
}
void Result::update(void) {
	framecnt++;
	(this->*stateUpdate)();
}
void Result::draw(void) {
	(this->*stateDraw)();
}

void Result::changeFontAlpha(void) {
	constexpr int CYCLE = 3000;
	const uint64 t = Time::GetMillisec();
	alphaFont = Sin(t % CYCLE / static_cast<double>(CYCLE) * Math::TwoPi) * 0.42 + 0.58;
}

//ゲームクリア用
void Result::imNumberInit() {
	imnumber = new ImageNumber(U"resources/images/items/num/num.png", NUMIM_WIDTH, NUMIM_HEIGHT);
	numEffect = new FlipEffect(U"resources/images/items/num/num.png", NUMIM_WIDTH, NUMIM_HEIGHT, 0, 0, NUMBER_SWTIME);
}

int Result::calcScore(JUDGE::JudgeCount &jc) {//スコア計算
	constexpr int weight[JUDGE::TYPE_SIZE] = {100, 70, 50, 0};
	int score = 0;
	for (int i = 0; i < JUDGE::TYPE_SIZE; i++) {
		score += weight[i] * jc.cnt[i];
	}
	return score;
}
void Result::successUpdate(void) {
	scoreEffect();
}
void Result::successDraw(void) {
	TextureAsset(U"back").drawAt(Window::Width() / 2, Window::Height() / 2);
	imnumber->draw();
	numEffect->draw();
}

void Result::scoreEffect(void) {
	static int canChange = 2;
	static int wordCnt = scoreStr.size() -1;
	static double prevtime = -100;

	if (wordCnt < 0) {//全て確定しているならば終了させる
		return;
	}
	if (stopwatch.msF() - prevtime >= NUM_ENDTIME) {
		if (canChange == 0) {
			imnumber->add(scoreStr.at(wordCnt) - U'0', SCOREX + wordCnt * NUMIM_WIDTH, SCOREY);
			wordCnt--;
			canChange = 2;
		}
		else if (canChange > 0) {
			numEffect->play(SCOREX + wordCnt * NUMIM_WIDTH, SCOREY);
			canChange--;
			prevtime = stopwatch.msF();
		}
	}

	//メモ　ゲームエフェクトで生存判定をとれるように変更する　生存何回で確定にする
	/*
	if (framecnt % frameend == 0) {//数字を確定させる
		scoreDraw.at(wordCnt) = scoreStr.at(wordCnt);
		wordCnt--;
	}
	*/
}
//ゲームオーバー用
void Result::failedUpdate(void) {
	//背景や文字を時間経過で表示させるための処理
	if (framecnt <= alphatime) {
		alphaBack = (double)framecnt / alphatime;
	}
	else {
		changeFontAlpha();
	}
}

void Result::failedDraw(void) {
	TextureAsset(U"back").drawAt(Window::Width() / 2, Window::Height() / 2, AlphaF(alphaBack));//背景描画
	FontAsset(U"font")(U"～ Escキーで終了 ～").drawAt(Window::Width() / 2, Window::Height() - 150, AlphaF(alphaFont));
}



ImageNumber::ImageNumber(FilePath path, int w, int h) {//横一列に並んでいる画像
	Image numberBase = Image(path);
	int wc = numberBase.width() / w;
	int wh = numberBase.height() / h;
	for (int i = 0; i < 10; i++) {
		imNumber[i] = Texture(numberBase.clipped(i * w, 0, w, h));
	}
}
ImageNumber::~ImageNumber() {

}
void ImageNumber::add(int num, int x, int y) {
	NumPoint tmp;
	tmp.num = num;
	tmp.x = x;
	tmp.y = y;
	numberp.push_back(tmp);
}
void ImageNumber::draw() {
	for (int i = 0; i < numberp.size(); i++) {
		imNumber[numberp.at(i).num].drawAt(numberp.at(i).x, numberp.at(i).y);
	}
}

