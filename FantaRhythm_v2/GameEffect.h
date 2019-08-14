#pragma once
#include "FantaRhythm_v2.h"

/*
namespace SE {
	enum NAME {
		HIT
	};
	enum TYPE {//同じSEが複数再生された時の処理
		DUL,	//重複
		REP,	//最初から再生
		IGN		//無視
	};
}
namespace VE {
	enum NAME {
		HIT
	};
}
namespace ME {
	enum NAME {
		HIT
	};
}

class GameEffect {
public:
	static void playSE(SE::NAME name, SE::TYPE type);	//効果音
	static void playVE(VE::NAME name);	//視覚演出
	static void playME(ME::NAME name);	//効果音と視覚演出のセット
};
*/

class SE {
	Audio* sound;
public:
	SE(FilePath& path);
	~SE();
	void play();//SEを再生する。重複して再生されようとした時、最初から再生しなおす
};

class mapflip {	//渡されたマップチップ画像からチップ画像を切り出す
	Texture* flip;
	int xFlipWidth;
	int yFlipHeight;
	int xMapWidth;
	int yMapHeight;
	int nowPosX;
	int nowPosY;
public:
	mapflip(const FilePath& path, int xFlipWidth, int yFlipHeight);
	~mapflip();
	bool mapflip::nextFlip();	//次の位置のチップ画像をセット。次がない(最後)ならfalseを返す
	TextureRegion getFlip();	//チップ画像を返す
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
	int firstX, firstY;
	int firstSize;
	int x2, y2;
	fractal(int size, int x, int y);
	bool update(double t);
private:
	void drawTriangle(double t, int posX, int posY, int size, int x2, int y2);
};

