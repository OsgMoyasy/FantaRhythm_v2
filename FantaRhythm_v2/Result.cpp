#include "Result.h"
#include <vector>
constexpr double NUMBER_SWTIME = 0.05;						//数字エフェクトが切り替わる時間
constexpr double NUM_MAXTIME = NUMBER_SWTIME * 9 * 1000;	//数字が確定するまでの時間
constexpr int NUMIM_WIDTH = 50, NUMIM_HEIGHT = 75;
constexpr int SCOREX = 800, SCOREY = 130;
const String numberImPath = U"resources/images/items/num/num2.png";


Result::Result(JUDGE::JudgeCount judgeCnt, int totalDamage, bool isClear) {
	this->judgeCnt = judgeCnt;
	this->totalDamage = totalDamage;
	this->isClear = isClear;
	stopwatch.start();
	alphaBack = 0;
	alphaFont = 0;

	FontAsset::Register(U"font", 50);
	FontAsset::Preload(U"font");

	if (isClear) {//ゲームクリア
		//テクスチャ初期化
		TextureAsset::Register(U"back", U"resources/images/back/result.png");

		//効果音初期化
		se = new SE(U"resources/musics/effects/Congratulations.wav");
		se->play();

		//クリア用ポインタ割り当て
		stateUpdate = &Result::successUpdate;
		stateDraw = &Result::successDraw;

		//スコア計算　文字列変換
		score = calcScore(this->judgeCnt);
		scoreStr = Format(score);
		damageStr = Format(totalDamage);

		//文字列を画像変換
		imNumberInit();
	}
	else {//ゲームオーバー
		TextureAsset::Register(U"back", U"resources/images/back/gameOver.jpg");
		//ゲームオーバー用ポインタ割り当て
		stateUpdate = &Result::failedUpdate;
		stateDraw = &Result::failedDraw;
	}
	TextureAsset::Preload(U"back");//背景のロード
}
Result::~Result(void) {
	FontAsset::Unregister(U"font");
	TextureAsset::UnregisterAll();
}
void Result::update(void) {
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
void Result::successUpdate(void) {
	double msF = stopwatch.msF();
	//順番に時間さで描画する為に描画終了しているならば次に進む
	if (scoreNumEffect->update(msF) == false) {
		if (damageNumEffect->update(msF) == false) {
			if (judgeUpdate() == false) {
				//全ての描画が終了
				changeFontAlpha();//Escで終了のメッセージ表示開始
			}
		}
	}
}
void Result::successDraw(void) {
	TextureAsset(U"back").drawAt(Window::Width() / 2, Window::Height() / 2);
	scoreNumEffect->draw();
	damageNumEffect->draw();
	judgeImNum->draw();
	FontAsset(U"font")(U"～ Escキーで終了 ～").drawAt(Window::Width() / 2, Window::Height() - 60, ColorF(0.0, 0.0, 0.0, alphaFont));
}

int Result::calcScore(JUDGE::JudgeCount& jc) {//スコア計算 判定の数と重みを掛けた総和をスコアとする
	constexpr int weight[JUDGE::TYPE_SIZE] = { 100, 70, 50, 0 };//重み [perfect, great, good, bad]
	int score = 0;
	for (int i = 0; i < JUDGE::TYPE_SIZE; i++) {
		score += weight[i] * jc.cnt[i];
	}
	return score;
}

void Result::imNumberInit() {
	scoreNumEffect = new NumWithEffect(numberImPath, scoreStr, 1, SCOREX, SCOREY, NUMIM_WIDTH, NUMIM_HEIGHT, NUMBER_SWTIME);
	damageNumEffect = new NumWithEffect(numberImPath, damageStr, 1, SCOREX, SCOREY + NUMIM_HEIGHT, NUMIM_WIDTH, NUMIM_HEIGHT, NUMBER_SWTIME);
	judgeImNum = new ImageNumber(numberImPath, NUMIM_WIDTH, NUMIM_HEIGHT);
}

bool Result::judgeUpdate() {
	static int row = 0;//1 = damage 2 = parfect・・・
	static double prevtime = stopwatch.msF();
	if (row >= JUDGE::TYPE_SIZE) {//全て追加し終わったら
		return false;
	}
	if (stopwatch.msF() - prevtime >= NUM_MAXTIME ) {//追加させる時間が来たら数字を追加し下方向へ
		judgeImNum->addMulti(judgeCnt.cnt[row], SCOREX, SCOREY + NUMIM_HEIGHT * (row + 2));
		prevtime = stopwatch.msF();
		row++;
	}
	return true;
}

//ゲームオーバー用
void Result::failedUpdate(void) {
	constexpr int alphaBackMaxMs = 3000;//背景画像のアルファ値が元に戻るまでの時間
	//背景や文字を時間経過で表示させるための処理
	if (stopwatch.ms() <= alphaBackMaxMs) {
		alphaBack = (double)stopwatch.ms() / alphaBackMaxMs;
	}
	else {
		changeFontAlpha();
	}
}

void Result::failedDraw(void) {
	TextureAsset(U"back").drawAt(Window::Width() / 2, Window::Height() / 2, AlphaF(alphaBack));//背景描画
	FontAsset(U"font")(U"～ Escキーで終了 ～").drawAt(Window::Width() / 2, Window::Height() - 100, AlphaF(alphaFont));
}



