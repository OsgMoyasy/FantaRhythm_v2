#pragma once
#include <Siv3D.hpp>

class Gauge {
public:
	Gauge(double x, double y, double w, double h, double maxRatio, Color backColor, Color frontColor);
	~Gauge(void);
	void update(double ratio);
	void draw(void);
	void draw(double y);
private:
	double x, y;//�`��ʒu�i����j
	double h, w;//���@����
	const double maxRatio;//���݂̃Q�[�W�̊����v�Z����Ƃ��A�ő�̒l
	double currentW;//�v�Z���ꂽ��̕�

	Color backColor, frontColor;

	void calcCurrentW(double ratio);
};