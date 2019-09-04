#pragma once
#include "FantaRhythm_v2.h"

class MusicManager {
private:
	static Audio* audio;

public:
	MusicManager() = delete;
	static void playMusicPrev(String path);
	static void playMusicGame(String path);

	static double getMusicTime();//今の再生時間をms単位で取得する
	static double getMilliSec();//1msをサンプリングレートから取得/*改変*/
};