#include "QrRead.h"
QrRead::QrRead(void) {
	TextureAsset::Register(U"qrreadback", U"resources/images/back/BackScreen.jpg");
	TextureAsset::Preload(U"qrreadback");
	TextureAsset::Register(U"qrreadmsg", U"resources/images/items/massage.png");
	TextureAsset::Preload(U"qrreadmsg");

	FontAsset::Register(U"qrreadfont", 70);
	FontAsset::Preload(U"qrreadfont");

	msgX = Window::Width() / 2;
	msgY = 100;
	msg = U"QRをかざしてください";
	readText = U"";
	webcam = Webcam(0);
	client = new HttpClient();
	isRead = false;
	changeFlag = true;
}
QrRead::~QrRead(void) {
	TextureAsset::Unregister(U"qrreadback");
	TextureAsset::Unregister(U"qrreadmsg");
	FontAsset::Unregister(U"qrreadfont");
	th.join();
	delete client;
}
void QrRead::start(void) {
	if (!webcam.start()) {
		msg = U"Webカメラが接続されていません。";
	}
}
void QrRead::update(void) {
	if (!isRead) {
		if (webcam.hasNewFrame()) {
			webcam.getFrame(image);
			camtexture.fill(image);
			QRContent qr;
			QRDecoder decoder;
			if (decoder.decode(image, qr)) {
				readText = qr.text;
				isRead = true;
				webcam.stop();
			}
		}
	}
	else {
		//読み込みが終了しネットワーク送受信も完了したら移行させる
		//HTTP GET 取得するファイルパス リクエスト先IP
		if (client->getStatus() == S_NONE) {
			std::vector<std::string> chaNum;
			std::stringstream ss{ readText.narrow() };
			std::string buf;
			while (std::getline(ss, buf, ',')) {
				chaNum.push_back(buf);
			}
			std::stringstream st;
			st << "/json?cha1=" << chaNum[0] << "&cha2=" << chaNum[1] << "&cha3=" << chaNum[2] << "&cha4=" << chaNum[3];
			th = std::thread(&HttpClient::Get, client, "/get", "httpbin.org");
		}
		else if(client->getStatus() == S_FINISH){
			if (changeFlag) {
				SceneManager::setNextScene(SceneManager::SCENE_SELECTMUSIC);
				changeFlag = false;
			}
		}

	}
}
void QrRead::draw(void) {
	TextureAsset(U"qrreadback").draw();
	TextureAsset(U"qrreadmsg").drawAt(msgX, msgY);
	FontAsset(U"qrreadfont")(msg).drawAt(msgX, msgY, ColorF(0,0,0));
	if (camtexture) {
		camtexture.drawAt(Window::Width() / 2, Window::Height()/2 + msgY);
	}
	Print << readText;
}