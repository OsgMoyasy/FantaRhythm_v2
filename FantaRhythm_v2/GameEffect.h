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
	void play();
};

class mapflip {	//渡されたマップチップ画像からチップ画像を切り出す
	Texture* map;
	int yFlipHeight;
	int xFlipWidth;
	int yMapHeight;
	int xMapWidth;
	int nowPosX;
	int nowPosY;
public:
	mapflip(const FilePath& path, int yFlipHeight, int xFlipWidth);
	~mapflip();
	bool mapflip::nextFlip();
	TextureRegion* flipFromMap();	//呼ばれる度に次の位置のチップ画像のポインタを返す
	inline TextureRegion* operator()() { return flipFromMap(); }
};

class flipMovie :IEffect {
	mapflip* flip;
	int drawX;
	int drawY;
public:
	flipMovie(const FilePath& path, int yFlipHeight, int xFlipWidth, int drawX, int drawY);
	~flipMovie();
	bool update(double t);
};