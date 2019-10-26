#pragma once
#include <Siv3D.hpp>
#include <vector>
#include "GameEffect.h"

//数字を画像表示するためのヘッダー
//ImageNumber 数字を画像表示
//NumWithEffect エフェクト付き数字画像表示

class ImageNumber {
private:
	static Texture imNumber[10];
	int imw, imh;

	typedef struct _NumPoint {
		int x, y;
		int num;
	}NumPoint;

	std::vector<NumPoint> numberp;//追加された数字を全て保持


public:
	ImageNumber(FilePath path, int w, int h);
	ImageNumber(FilePath path);
	~ImageNumber();
	void addOne(int num, int x, int y);//数字を１文字追加する
	void ImageNumber::addMulti(int num, int x, int y);//数字列を追加する
	void clear();
	void draw(double alphaF = 1);

};

class NumWithEffect {
private:
	String numStr;
	class FlipEffect* numEffect;
	int fixedAtTime;
	int currentWord;//現在確定させようとしている数字
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

class ComboImNumber {
private:
	ImageNumber* imnumber;
	Texture* comboIm;
	int x, y;
	const double ALPHAMS;
	double alphaMs;	//消えるまでの時間
	double alpha;
	Stopwatch stopwatch;
	
	void calcAlpha();
public:
	ComboImNumber(int x, int y, double alphaMs);
	~ComboImNumber();
	void update();
	void draw();
	void setCombo(int combo);
	void resetCombo();

};