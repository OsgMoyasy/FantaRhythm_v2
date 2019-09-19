#pragma once

#include <Siv3D.hpp>
#include <vector>
#include "GameEffect.h"

class ImageNumber {
private:
	Texture imNumber[10];
	int imw, imh;

	typedef struct _NumPoint {
		int x, y;
		int num;
	}NumPoint;

	std::vector<NumPoint> numberp;//’Ç‰Á‚³‚ê‚½”š‚ğ‘S‚Ä•Û


public:
	ImageNumber(FilePath path, int w, int h);
	~ImageNumber();
	void addOne(int num, int x, int y);//”š‚ğ‚P•¶š’Ç‰Á‚·‚é
	void ImageNumber::addMulti(int num, int x, int y);//”š—ñ‚ğ’Ç‰Á‚·‚é
	void draw();

};

class NumWithEffect {
private:
	String numStr;
	class FlipEffect* numEffect;
	int fixedAtTime;
	int currentWord;//Œ»İŠm’è‚³‚¹‚æ‚¤‚Æ‚µ‚Ä‚¢‚é”š
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