#include "Gauge.h"
Gauge::Gauge(double x, double y, double w, double h, double maxRatio, Color backColor, Color frontColor):maxRatio(maxRatio) {
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->backColor = backColor;
	this->frontColor = frontColor;
	currentW = w;
}
Gauge::~Gauge(void) {

}

void Gauge::update(double ratio) {
	calcCurrentW(ratio);
}
void Gauge::draw(void) {
	Rect(x, y, w, h).draw(backColor);
	Rect(x, y, currentW, h).draw(frontColor);
}
void Gauge::draw(double y) {
	Rect(x, y, w, h).draw(backColor);
	Rect(x, y, currentW, h).draw(frontColor);
}

void Gauge::calcCurrentW(double ratio) {
	double per = (double)ratio / maxRatio;
	currentW = per * w;
}