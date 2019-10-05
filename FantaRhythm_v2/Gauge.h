#pragma once
#include <Siv3D.hpp>

class Gauge {
public:
	Gauge(double x, double y, const String& imGagePath, double maxRatio, Color backColor, Color frontColor);
	~Gauge(void);
	void update(double ratio);
	void draw(void);
	void draw(double y);
private:
	Texture imChaGauge;
	double x, y;//描画位置（左上）
	double h, w;//幅　高さ
	const double maxRatio;//現在のゲージの割合計算するとき、最大の値
	double currentW;//計算された後の幅

	Color backColor, frontColor;

	void calcCurrentW(double ratio);
};