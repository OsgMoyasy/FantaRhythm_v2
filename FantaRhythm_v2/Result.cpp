#include "Result.h"
#include <vector>
constexpr double NUMBER_SWTIME = 0.05;
constexpr double NUM_ENDTIME = NUMBER_SWTIME * 9 * 1000;
constexpr int NUMIM_WIDTH = 50, NUMIM_HEIGHT = 75;
constexpr int SCOREX = 800, SCOREY = 130;

constexpr int JUDGEXY[4][2] = { {50,100,} };

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
		TextureAsset::Register(U"back", U"resources/images/back/result.png");
		imNumberInit();
		//効果音初期化
		se = new SE(U"resources/musics/effects/Congratulations.wav");
		se->play();

		stateUpdate = &Result::successUpdate;
		stateDraw = &Result::successDraw;
		int score = calcScore(this->judgeCnt);
		scoreStr = Format(score);
		damageStr = Format(totalDamage);
		

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
	imnumber = new ImageNumber(U"resources/images/items/num/num2.png", NUMIM_WIDTH, NUMIM_HEIGHT);
	numEffect = new FlipEffect(U"resources/images/items/num/num2.png", NUMIM_WIDTH, NUMIM_HEIGHT, 0, 0, NUMBER_SWTIME);
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
	if (scoreEffect() == false) {
		if (damageEffect() == false) {
			if (judgeEffect() == false) {

			}
		}
	}
}
void Result::successDraw(void) {
	TextureAsset(U"back").drawAt(Window::Width() / 2, Window::Height() / 2);
	imnumber->draw();
	numEffect->draw();
}


bool Result::scoreEffect() {
	static int canChange = 1;
	static int currentWord = scoreStr.size() -1;//現在確定させようとしている数字
	static double prevtime = -100;

	if (currentWord < 0) {//全て確定しているならばダメージ以下の描画に入る
		return false;
	}
	if (stopwatch.msF() - prevtime >= NUM_ENDTIME) {
		if (canChange == 0) {
			imnumber->addOne(scoreStr.at(currentWord) - U'0', (SCOREX - scoreStr.size() * NUMIM_WIDTH) + currentWord * NUMIM_WIDTH, SCOREY);
			currentWord--;
			canChange = 1;
		}
		else if (canChange > 0) {
			numEffect->play((SCOREX - scoreStr.size() * NUMIM_WIDTH) + currentWord * NUMIM_WIDTH, SCOREY);
			canChange--;
			prevtime = stopwatch.msF();
		}
	}
	return true;
}

bool Result::damageEffect() {
	static int canChange = 1;
	static int currentWord = damageStr.size() - 1;//現在確定させようとしている数字
	static double prevtime = -100;

	if (currentWord < 0) {//全て確定しているならばダメージ以下の描画に入る
		return false;
	}
	if (stopwatch.msF() - prevtime >= NUM_ENDTIME) {
		if (canChange == 0) {
			imnumber->addOne(damageStr.at(currentWord) - U'0', (SCOREX - damageStr.size() * NUMIM_WIDTH) + currentWord * NUMIM_WIDTH, SCOREY + NUMIM_HEIGHT);
			currentWord--;
			canChange = 1;
		}
		else if (canChange > 0) {
			numEffect->play((SCOREX - damageStr.size() * NUMIM_WIDTH) + currentWord * NUMIM_WIDTH, SCOREY + NUMIM_HEIGHT);
			canChange--;
			prevtime = stopwatch.msF();
		}
	}
	return true;
}

bool Result::judgeEffect() {
	static int row = 0;//1 = damage 2 = parfect・・・
	static double prevtime = stopwatch.msF();
	if (row >= 4) {
		return false;
	}
	if (stopwatch.msF() - prevtime >= NUM_ENDTIME && row < 4) {
		imnumber->addMulti(judgeCnt.cnt[row], SCOREX, SCOREY + NUMIM_HEIGHT * (row + 2));
		prevtime = stopwatch.msF();
		row++;
	}
	return true;
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
	for (int i = 0; i < 10; i++) {
		imNumber[i] = Texture(numberBase.clipped(i * w, 0, w, h));
	}
	imw = w;
	imh = h;
}
ImageNumber::~ImageNumber() {

}
void ImageNumber::addOne(int num, int x, int y) {
	NumPoint tmp;
	tmp.num = num;
	tmp.x = x;
	tmp.y = y;
	numberp.push_back(tmp);
}

void ImageNumber::addMulti(int num, int x, int y) {
	constexpr char32_t zero = U'0';
	String nums = Format(num);
	for (int i = 0; i < nums.size(); i++) {
		int n = nums.at(i) - zero;
		addOne(n, (x - nums.size() * NUMIM_WIDTH) + imw * i, y);
	}
}

void ImageNumber::draw() {
	for (int i = 0; i < numberp.size(); i++) {
		imNumber[numberp.at(i).num].drawAt(numberp.at(i).x, numberp.at(i).y);
	}
}

