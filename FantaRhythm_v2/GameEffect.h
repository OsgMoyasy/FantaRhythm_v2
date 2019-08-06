#pragma once
#include "FantaRhythm_v2.h"

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