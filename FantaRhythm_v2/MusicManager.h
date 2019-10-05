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

	static double getMusicTime();//���̍Đ����Ԃ�ms�P�ʂŎ擾����
	static double getMilliSec();//1ms���T���v�����O���[�g����擾/*����*/
	static bool musicEndCheck();//�Đ����I�����Ă��邩���f�@�Ȃ��I�����Ă���T�b�قǑ��������Ԃ��I������
};