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
	if (MyKey::getDecisionKey()) {//ゲストユーザー
		SceneManager::setNextScene(SceneManager::SCENE_SELECTMUSIC);
		/*
		isRead = true;
		readText = U"gest";
		webcam.stop();
		*/
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
			std::stringstream st;
			RankingData::setUser_id(readText);
			st << "/getChar?user_hash="+readText.narrow();
			th = std::thread(&HttpClient::Get, client, st.str(), "192.168.10.3",std::ref(th_status));
			msg = U"サーバーと通信中";
			
		}
		else if(th_status == TH_FINISH){
			if (isChange) {
				JSONReader json(U"test.json");
				JSONArrayView jsonArray = json[U"user"][U"role"].arrayView();
				RankingData::setName(json[U"user"][U"nickname"].get<String>());
				for (int i = 0; i < 4; i++) {
					RankingData::setChar_id(i, jsonArray[i][U"character_id"].get<int>());
				}

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