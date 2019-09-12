#include "Result.h"

Result::Result(JUDGE::JudgeCount judgeCnt, int totalDamage, bool clearFlag) {
	this->judgeCnt = judgeCnt;
	this->totalDamage = totalDamage;
	this->clearFlag = clearFlag;

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
		feffect = new FlipEffect(U"resources/images/effects/num2.png", 180, 215, 0, 0);
		//効果音初期化
		se = new SE(U"resources/musics/effects/Congratulations.wav");
		se->play();

		stateUpdate = &Result::successUpdate;
		stateDraw = &Result::successDraw;
		int score = calcScore(this->judgeCnt);
		scoreStr = Format(score);
		scoreDraw = U"               ";
		feffect->play(100,100);
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
	constexpr int w = 180, h = 215;
	Image numberBase = Image(U"resources/images/effect/num2.png");
	int wc = numberBase.width() / w;
	int wh = numberBase.height() / h;
	for (int y = 0; y < wh; y++) {
		for (int x = 0; x < wc; x++) {
			imNumber[x + wc * y] = Texture(numberBase.clipped(x * wc, y * wh, w, h));
		}
	}
	
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
	FontAsset(U"subfont")(U"Score   ::"+ scoreDraw).draw(Window::Width() / 2 - 100, 150,Color(0x000000));
	FontAsset(U"subfont")(U"Perfect ::" + Format(this->judgeCnt.cnt[JUDGE::PERFECT])).draw(Window::Width() / 2 - 100, 190, Color(0x000000));
	FontAsset(U"subfont")(U"Great   ::" + Format(this->judgeCnt.cnt[JUDGE::GREAT])).draw(Window::Width() / 2 - 100, 220, Color(0x000000));
	FontAsset(U"subfont")(U"Good    ::" + Format(this->judgeCnt.cnt[JUDGE::GOOD])).draw(Window::Width() / 2 - 100, 260, Color(0x000000));
	FontAsset(U"subfont")(U"Bad     ::" + Format(this->judgeCnt.cnt[JUDGE::BAD])).draw(Window::Width() / 2 - 100, 300, Color(0x000000));
	feffect->draw();
}

void Result::scoreEffect(void) {
	constexpr int frameend = 60 * 1;//数値を決定するまでのフレーム数
	constexpr int framespace = 3; //数値が移り変わるフレーム数

	static int wordcnt = scoreStr.size() -1;
	static int number = 0;	//移り変わっている数字

	if (wordcnt < 0) {//全て確定しているならば終了させる
		return;
	}

	if (framecnt % framespace == 0) {//0-9まで間隔空けて変更
		number++;
		number %= 10;
		String tmp = Format(number);//一発で出来る様に要変更
		scoreDraw.at(wordcnt) = tmp.at(0);
	}
	if (framecnt % frameend == 0) {//数字を確定させる
		scoreDraw.at(wordcnt) = scoreStr.at(wordcnt);
		wordcnt--;
	}
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