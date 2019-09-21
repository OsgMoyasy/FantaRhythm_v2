#pragma once
#include <Siv3D.hpp>

class SE {
public:
	SE(const FilePath& path);
	~SE();
	void play();//SEを再生する。重複して再生されようとした時、最初から再生しなおす
private:
	Audio* sound;
};

class MapFlip {	//渡された大きい画像から切り出した画像を返す
public:
	MapFlip(Texture map, int xFlipWidth, int yFlipHeight);
	bool nextFlip();			//次の切り出し画像の位置にセット。次がない(最後)ならfalseを返す
	TextureRegion getFlip();	//切り出し画像を返す
private:
	Texture map;	//大きい画像
	int xMapWidth, yMapHeight;		//大きい画像の大きさ
	int xFlipWidth, yFlipHeight;	//切り出しサイズ
	int xNowPos, yNowPos;			//現在の画像切り出し位置
};

struct FlipMovie :IEffect {//MapFlipを利用してパラパラ漫画を作るエフェクト
	FlipMovie(Texture map, int xFlipWidth, int yFlipHeight, int xDraw = 0, int yDraw = 0, double switchBetween = 0.03);
	~FlipMovie();
	bool update(double t);
private:
	MapFlip* mapflip;
	int xDraw, yDraw;			//描画位置
	double switchTime;	//最後に切り出し画像を切り替えた時間
	double switchBetween;
};

class FlipEffect {//同じFlipMovieエフェクトを繰り返し再生する際のリソースを保持する。SEも再生できる。
public:
	FlipEffect(const FilePath& path, int xFlipWidth, int yFlipHeight, int xDraw = 0, int yDraw = 0, double switchBetween = 0.03);
	~FlipEffect();
	void setTexture(const FilePath& path, int xFlipWidth, int yFlipHeight);	//画像をセット("画像パス",切り出しサイズ)
	void setPos(int xDraw, int yDraw);	//描画位置をセット
	void setSE(FilePath& path);			//SEをセット
	void play();						//再生開始
	void play(int xDraw, int yDraw);	//再生開始(描画位置)
	void draw();	//描画(フレーム毎に呼び出す必要あり)
private:
	//エフェクト関係
	Effect effect;
	Texture map;
	int xFlipWidth, yFlipHeight;	//切り出しサイズ
	int xDraw, yDraw;				//描画位置
	double switchBetween;

	//SE関係
	SE* se;
	bool existSE;	//SEが設定されているか
};

template<typename T>
class FlipMap {//FlipEffectをunordered_mapで管理する為のクラス。キーに使う型は自由。
public:
	~FlipMap();
	void set(T name, const FilePath& path, int xFlipWidth, int yFlipHeight, int xDraw = 0, int yDraw = 0);	//使用するエフェクトを登録
	FlipEffect* get(T name);	//エフェクトを指定して取得
	void draw();					//再生中の全てのエフェクトを描画
private:
	std::unordered_map<T, FlipEffect*> usingEffect;
};
#include"FlipSet.h"

/*
struct Fractal :IEffect {//フラクタル模様を作り出すエフェクト
	Fractal(int size, int x, int y);
	bool update(double t);
private:
	int xFirst, yFirst;
	int firstSize;
	int x2, y2;

	void drawTriangle(double t, int xPos, int yPos, int size, int x2, int y2);
};
*/

