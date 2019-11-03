#include "QrRead.h"
QrRead::QrRead(void) {
	TextureAsset::Register(U"qrreadback", U"resources/images/back/BackScreen.jpg", AssetParameter::LoadAsync());
	TextureAsset::Register(U"qrreadmsg", U"resources/images/items/massage.png", AssetParameter::LoadAsync());

	FontAsset::Register(U"qrreadfont", 50);
	FontAsset::Preload(U"qrreadfont");

	msgX = Window::Width() / 2;
	msgY = 100;
	msg = U"QR���������Ă�������";
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
		msg = U"Web�J�������ڑ�����Ă��܂���B";
	}
}
void QrRead::update(void) {
	if (MyKey::getReturnKey()) {
		SceneManager::setNextScene(SceneManager::SCENE_TITLE);
	}
	if (MyKey::getDecisionKey()) {//�Q�X�g���[�U�[
		readText = U"gest";
		RankingData::setName(readText);
		RankingData::setUser_id(readText);
		client->filepath = "gest.json";
		//webcam.stop();
		SceneManager::setNextScene(SceneManager::SCENE_SELECTMUSIC);
		
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
				//webcam.stop();
			}
		}
	}
	else {
		//�ǂݍ��݂��I�����l�b�g���[�N����M������������ڍs������
		//HTTP GET �擾����t�@�C���p�X ���N�G�X�g��IP 
		if (th_status == TH_NONE) {		
			std::stringstream st;
			RankingData::setUser_id(readText);
			st << "/users/"+readText.narrow();
			th = std::thread(&HttpClient::Get, client, st.str(), "127.0.0.1",std::ref(th_status));
			msg = U"�T�[�o�[�ƒʐM��";
			
		}
		else if(th_status == TH_FINISH){
			if (isChange) {
				JSONReader json(U"chardata.json");
				JSONArrayView jsonArray = json[U"role"].arrayView();
				RankingData::setName(json[U"nickname"].get<String>());
				for (int i = 0; i < 4; i++) {
					RankingData::setChar_id(i, jsonArray[i][U"id"].get<int>());
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
	//Print << readText;
}