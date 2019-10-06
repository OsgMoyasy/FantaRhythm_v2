#include "ImageNumber.h"

Texture ImageNumber::imNumber[10];

ImageNumber::ImageNumber(FilePath path, int w, int h) {//横一列に並んでいる画像
	Image numberBase = Image(path);
	for (int i = 0; i < 10; i++) {
		imNumber[i] = Texture(numberBase.clipped(i * w, 0, w, h));
	}
	imw = w;
	imh = h;
}
ImageNumber::~ImageNumber() {

}
void ImageNumber::addOne(int num, int x, int y) {
	NumPoint tmp;
	tmp.num = num;
	tmp.x = x;
	tmp.y = y;
	numberp.push_back(tmp);
}

void ImageNumber::addMulti(int num, int x, int y) {
	constexpr char32_t zero = U'0';
	String nums = Format(num);
	for (int i = 0; i < nums.size(); i++) {
		int n = nums.at(i) - zero;
		addOne(n, (x - (int)nums.size() * imw) + imw * i, y);
	}
}

void ImageNumber::draw() {
	for (int i = 0; i < numberp.size(); i++) {
		imNumber[numberp.at(i).num].drawAt(numberp.at(i).x, numberp.at(i).y);
	}
}

NumWithEffect::NumWithEffect(FilePath path, String numStr, int fixedAtTime, int x, int y, int w, int h, double switchMs) {
	this->imnumber = new ImageNumber(path, w, h);
	this->numStr = numStr;
	this->fixedAtTime = fixedAtTime;
	this->currentWord = (int)numStr.size() - 1;
	this->prevtime = -100;
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	numEffect = new FlipEffect(U"resources/images/items/num/num2.png", w, h, 0, 0, switchMs);
	maxMs = switchMs * 9 * 1000;
}
NumWithEffect::~NumWithEffect(void) {
	delete imnumber;
	delete numEffect;
}
bool NumWithEffect::update(double msF) {
	if (currentWord < 0) {//数字エフェクトの描画が終わったら
		return false;
	}
	if (msF - prevtime >= maxMs) {//切り替えるタイミングが来たら
		if (fixedAtTime == 0) {//確定させて次の数字へ(左方向)
			imnumber->addOne(numStr.at(currentWord) - U'0', (x - (int)numStr.size() * w) + currentWord * w, y);
			currentWord--;
			fixedAtTime = 1;
		}
		else if (fixedAtTime > 0) {//エフェクトを描画
			numEffect->play((x - (int)numStr.size() * w) + currentWord * w, y);
			fixedAtTime--;
			prevtime = msF;
		}
	}
	return true;
}
void NumWithEffect::draw(void) {
	imnumber->draw();
	numEffect->draw();
}

