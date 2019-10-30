#include "QrRead.h"
QrRead::QrRead(void) {
	TextureAsset::Register(U"qrreadback", U"resources/images/back/BackScreen.jpg", AssetParameter::LoadAsync());
	TextureAsset::Register(U"qrreadmsg", U"resources/images/items/massage.png", AssetParameter::LoadAsync());

	FontAsset::Register(U"qrreadfont", 50);
	FontAsset::Preload(U"qrreadfont");

	msgX = Window::Width() / 2;
	msgY = 100;
	msg = U"QRをかざしてください";
	readText = U"";
	client = new HttpClient();
	isRead = false;
	isChange = true;
	th_status = TH_NONE;
}

QrRead::~QrRead(void) {
	TextureAsset::Unregister(U"qrreadback");
	TextureAsset::Unregister(U"qrreadmsg");
	FontAsset::Unregister(U"qrreadfont");
	if (th.joinable()) {
		th.join();
	}
	delete client;
}

bool QrRead::isReady(void) {
	if (TextureAsset::IsReady(U"qrreadback") &&
		TextureAsset::IsReady(U"qrreadmsg")) {
		return true;
	}
	return false;
}

void QrRead::start(void) {
	if (!webcam.start()) {
		msg = U"Webカメラが接続されていません。";
	}
}
void QrRead::update(void) {
	if (MyKey::getReturnKey()) {
		SceneManager::setNextScene(SceneManager::SCENE_TITLE);
	}
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
		if (th_status == TH_NONE) {
			std::vector<std::string> chaNum;
			std::stringstream ss{ readText.narrow() };
			std::string buf;
			while (std::getline(ss, buf, ',')) {
				chaNum.push_back(buf);
			}
			
			PlayerName::setName(s3d::Unicode::Widen(chaNum.at(chaNum.size() - 1)));
			std::stringstream st;
			st << "/getChar?cha1=" << chaNum[0] << "&cha2=" << chaNum[1] << "&cha3=" << chaNum[2] << "&cha4=" << chaNum[3];
			th = std::thread(&HttpClient::Get, client, st.str(), "127.0.0.1",std::ref(th_status));
			msg = U"サーバーと通信中";
			
		}
		else if(th_status == TH_FINISH){
			if (isChange) {
				SceneManager::setNextScene(SceneManager::SCENE_SELECTMUSIC);
				isChange = false;
			}
		}

	}
}
void QrRead::draw(void) {
	TextureAsset(U"qrreadback").draw();
	TextureAsset(U"qrreadmsg").drawAt(msgX, msgY);
	FontAsset(U"qrreadfont")(msg).drawAt(msgX, msgY - 10, ColorF(0,0,0));
	if (camtexture) {
		camtexture.drawAt(Window::Width() / 2, Window::Height()/2 + msgY);
	}
	Print << readText;
}