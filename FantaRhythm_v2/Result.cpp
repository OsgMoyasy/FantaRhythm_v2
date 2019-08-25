#include "Result.h"


#define CYCLE 3000
#define TWOPI 6.28318

Result::Result(JUDGE::JudgeCount judgecnt, int totaldmg, bool cflag) {
	judgecount = judgecnt;
	totaldamage = totaldmg;
	clearflag = cflag;

	FontAsset::Register(U"font", 50);
	FontAsset::Preload(U"font");
	FontAsset::Register(U"subfont", 30);
	FontAsset::Preload(U"subfont");
	if (clearflag) {//ゲームクリア
		TextureAsset::Register(U"back", U"resources/images/back/BackScreen.jpg");
		stateUpdate = &Result::successUpdate;
		stateDraw = &Result::successDraw;
		int score = calcScore(judgecount);
		score = 1192;
		scoreStr = Format(score);
		scoreDraw = U"               ";
	}
	else {//ゲームオーバー
		TextureAsset::Register(U"back", U"resources/images/back/gameOver.jpg");
		stateUpdate = &Result::failedUpdate;
		stateDraw = &Result::failedDraw;

		framecnt = 0;
		alpha = 0;
		alphafont = 0;
	}
	TextureAsset::Preload(U"back");//背景のロード
}
Result::~Result(void) {
	FontAsset::Unregister(U"font");
	FontAsset::Unregister(U"subfont");
	TextureAsset::UnregisterAll();
}
void Result::update(void) {
	(this->*stateUpdate)();
}
void Result::draw(void) {
	

	(this->*stateDraw)();
}

//ゲームクリア用
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
	FontAsset(U"subfont")(U"Score   ::"+ scoreDraw).draw(Window::Width() / 2 - 100, 150,Color(0x000000));
	FontAsset(U"subfont")(U"Perfect ::" + Format(judgecount.cnt[JUDGE::PERFECT])).draw(Window::Width() / 2 - 100, 190, Color(0x000000));
	FontAsset(U"subfont")(U"Great   ::" + Format(judgecount.cnt[JUDGE::GREAT])).draw(Window::Width() / 2 - 100, 220, Color(0x000000));
	FontAsset(U"subfont")(U"Good    ::" + Format(judgecount.cnt[JUDGE::GOOD])).draw(Window::Width() / 2 - 100, 260, Color(0x000000));
	FontAsset(U"subfont")(U"Bad     ::" + Format(judgecount.cnt[JUDGE::BAD])).draw(Window::Width() / 2 - 100, 300, Color(0x000000));
}

void Result::scoreEffect(void) {
	static int cnt = 0;
	static int wordcnt = scoreStr.size() -1;
	static int number = 0;
	constexpr int frameend = 60 * 1;//数値を決定するまでの時間
	constexpr int framespace = 3;
	if (wordcnt < 0) {//全て確定しているならば終了
		return;
	}
  	cnt++;//フレーム数カウント

	//もう少し複雑になれば関数化
	if (cnt % framespace == 0) {//0-9まで間隔空けて変更
		number++;
		if (number > 9) {
			number = 0;
		}
		String tmp = Format(number);//要変更
		scoreDraw.at(wordcnt) = tmp.at(0);
	}
	if (cnt % frameend == 0) {//フレーム数が一定に達したら確定
		scoreDraw.at(wordcnt) = scoreStr.at(wordcnt);
		wordcnt--;
	}


}
//ゲームオーバー用
void Result::failedUpdate(void) {
	if (framecnt <= alphatime) {
		alpha = (double)framecnt / alphatime * 1;
		framecnt++;
	}
	else {
		//文字の描画開始
		const uint64 t = Time::GetMillisec();
		alphafont = Sin(t % CYCLE / static_cast<double>(CYCLE) * TWOPI) * 0.42 + 0.58;
	}
}
void Result::failedDraw(void) {
	TextureAsset(U"back").drawAt(Window::Width() / 2, Window::Height() / 2, AlphaF(alpha));//背景描画
	FontAsset(U"font")(U"～ Escキーで終了 ～").drawAt(Window::Width() / 2, Window::Height() - 150, AlphaF(alphafont));
}