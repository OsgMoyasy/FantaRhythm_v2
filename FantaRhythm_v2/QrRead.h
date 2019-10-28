#pragma once
#include <Siv3D.hpp>
#include <vector>
#include <thread>
#include"MyKey.h"
#include"Scene.h"
#include "HttpClient.h"
#include "SceneManager.h"
#include "PlayerName.h"
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
	bool isChange;
	TH_STATUS th_status;

	Webcam webcam;
	String msg;
	String readText;
	Image image;
	DynamicTexture camtexture;
	HttpClient* client;
	std::thread th;

};
