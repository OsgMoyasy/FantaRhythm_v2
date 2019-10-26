#pragma once
#include<Siv3D.hpp>
#include"MyKey.h"
#include"Scene.h"

class QrRead : public Scene {
public:
	QrRead(void);
	~QrRead(void);
	void start(void);
	void update(void);
	void draw(void);
private:
	int msgX;
	int msgY;
	bool isRead;

	Webcam webcam;
	String msg;
	String readText;
	Image image;
	DynamicTexture camtexture;

};
