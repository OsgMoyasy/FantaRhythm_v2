#pragma once
#include "FantaRhythm_v2.h"

class SE {
public:
	SE(FilePath& path);
	~SE();
	void play();//SEを再生する。重複して再生されようとした時、最初から再生しなおす
private:
	Audio* sound;
};

class mapflip {	//渡されたマップチップ画像からチップ画像を切り出す
public:
	mapflip(const FilePath& path, int xFlipWidth, int yFlipHeight);
	~mapflip();
	bool nextFlip();	//次の位置のチップ画像をセット。次がない(最後)ならfalseを返す
	TextureRegion getFlip();	//チップ画像を返す
private:
	Texture* flip;
	int xFlipWidth;
	int yFlipHeight;
	int xMapWidth;
	int yMapHeight;
	int nowPosX;
	int nowPosY;
};

struct flipMovie :IEffect {//mapflipを利用してパラパラ漫画を作るエフェクト
	flipMovie(const FilePath& path, int xFlipWidth, int yFlipHeight, int drawX, int drawY);
	~flipMovie();
	bool update(double t);
private:
	mapflip* flip;
	int drawX;
	int drawY;
};

struct fractal :IEffect {//フラクタル模様を作り出すエフェクト
	fractal(int size, int x, int y);
	bool update(double t);
private:
	int firstX, firstY;
	int firstSize;
	int x2, y2;

	void drawTriangle(double t, int posX, int posY, int size, int x2, int y2);
};

