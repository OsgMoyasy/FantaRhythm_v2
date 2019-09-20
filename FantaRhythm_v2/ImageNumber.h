#pragma once
#include <Siv3D.hpp>
#include <vector>
#include "GameEffect.h"

//�������摜�\�����邽�߂̃w�b�_�[
//ImageNumber �������摜�\��
//NumWithEffect �G�t�F�N�g�t�������摜�\��

class ImageNumber {
private:
	Texture imNumber[10];
	int imw, imh;

	typedef struct _NumPoint {
		int x, y;
		int num;
	}NumPoint;

	std::vector<NumPoint> numberp;//�ǉ����ꂽ������S�ĕێ�


public:
	ImageNumber(FilePath path, int w, int h);
	~ImageNumber();
	void addOne(int num, int x, int y);//�������P�����ǉ�����
	void ImageNumber::addMulti(int num, int x, int y);//�������ǉ�����
	void draw();

};

class NumWithEffect {
private:
	String numStr;
	class FlipEffect* numEffect;
	int fixedAtTime;
	int currentWord;//���݊m�肳���悤�Ƃ��Ă��鐔��
	double prevtime;
	ImageNumber* imnumber;
	int x, y, w, h;
	int maxMs;
public:
	NumWithEffect(FilePath path, String numStr, int fixedAtTime, int x, int y, int w, int h, double switchMs);
	~NumWithEffect(void);
	bool update(double msF);
	void draw(void);
};