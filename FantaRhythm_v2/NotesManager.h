#pragma once
# include <Siv3D.hpp> // OpenSiv3D v0.3.2
#include <vector>
#include <list>
#include <array>
#include "Observer.h"
#include "Judge.h"
#include "GameEffect.h"
#include "MusicManager.h"

class NotesManager {
private:
	static constexpr int LANESIZE = 4;
	enum class NOTESTYPE;
	struct Notes;


	int notewidth;
	
	class NotesSubject* notessubject;
	
	std::list<Notes> notelist[LANESIZE];
	using noteitr = std::list<Notes>::iterator;
	noteitr checkitr[LANESIZE];		//���肷�ׂ��C�e���[�^�ێ�
	noteitr displayitr[LANESIZE];	//�\�����ׂ��C�e���[�^�ێ�

	JUDGE::JudgeCount judgecount;

	int nowtime;	//���ݎ���[ms]

	//�{�^���֌W�̕ϐ�
    int down[LANESIZE];
	int press[LANESIZE];

	//�����O�m�[�c�p
	int pressedkey[LANESIZE];//�����ꂽ�{�^����ێ�

	//�`��֌W�̕ϐ�
	int laneStartX[LANESIZE];	//Start = ���[���̏�[
	int laneStartY;		
	int laneJudgeX[LANESIZE];	//Judge = ���[���Ɣ����������鏊
	int laneJudgeY;
	int laneGoalY;		//Goal = ����؂����m�[�c�̕\������߂鏊
	float timeRequired;	//�m�[�c�̏o�����画��܂ŗ���鎞��[ms]
	float notespeed;	//�m�[�c���x�̕␳�{��

	void plusItr(noteitr& itr);	//notelist�̃C�e���[�^��i�߂�

	void checkAttack(void);		//�{�^���̉����󋵂��m�F����
	struct ButtonandJudge;		//NoteisHit�֐��̉����p
	JUDGE::TYPE judgeType(int checktime);//����̃^�C�v��Ԃ�
	JUDGE::TYPE NoteisHit(int judgetime);

	void controlJudge(void);	//�m�[�c�̎�ޖ��ɔ���p�֐����Ăяo��
	void judgeNormal(int lane);	//�m�[�}���m�[�c�𔻒�
	void judgeLong(int lane);	//�����O�m�[�c�𔻒�


	void judgeEvent(JUDGE::TYPE type, int lane, bool next = true);
	void judgeLongEvent(JUDGE::TYPE type, int lane);
	
	double getProgress(int time);//���[����[���画����܂ł̐i�񂾊�����Ԃ�
	double progressByAngle(double progressRate);//���[���̊p�x�ɂ��␳��progressRate�ɍs��
	double getCurrentPosition(double startPos, double endPos, double progressRate);//���ݍ��W��Ԃ�
	double getScale(double currenty);//�g�嗦�v�Z

	void displayNormal(int lane, int time);				//�m�[�}���m�[�c��\��
	void displayLong(int lane, int time, int longtime);	//�����O�m�[�c��\��


	void setEvent(Massage msg, int val);
	
public:
	NotesManager(NotesSubject* sub,const String& difpath);
	~NotesManager();
	void update(void);
	void draw(void);
	JUDGE::JudgeCount* getJudgeCount(void);
};
