#pragma once
#include <Siv3D.hpp>

class MusicManager {
private:
	static Audio* audio;
	static Stopwatch stopwatch;

public:
	MusicManager() = delete;
	static void playMusicPrev(String path);
	static void setMusicGame(String path);
	static void pauseMusic();
	static void playMusic();
	static void setEndMusic(void);

	static double getMusicTime();//今の再生時間をms単位で取得する
	static double getMilliSec();//1msをサンプリングレートから取得/*改変*/
	static bool musicEndCheck();//再生が終了しているか判断　曲が終了してから５秒ほど足した時間が終了時間
};