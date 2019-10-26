#include "Gauge.h"
Gauge::Gauge(double x, double y,const String& imGaugePath, double maxRatio, Color backColor, Color frontColor):maxRatio(maxRatio) {
	imChaGauge = Texture( imGaugePath + U"/gauge.png");
	this->x = x;
	this->y = y;
	this->w = imChaGauge.width();
	this->h = imChaGauge.height();
	this->backColor = backColor;
	this->frontColor = frontColor;
	currentW = this->w;
}
Gauge::~Gauge(void) {

}

void Gauge::update(double ratio) {
	calcCurrentW(ratio);
}
void Gauge::draw(void) {
	Rect(x, y, w, h).draw(backColor);
	Rect(x, y, currentW, h).draw(frontColor);
	imChaGauge.draw(x,y);
}
void Gauge::draw(double y) {
	Rect(x, y + 2, w, h - 2).draw(backColor);
	Rect(x, y + 2, currentW, h - 2).draw(frontColor);
	imChaGauge.draw(x, y);
}

void Gauge::calcCurrentW(double ratio) {
	double per = (double)ratio / maxRatio;
	currentW = per * w;
}