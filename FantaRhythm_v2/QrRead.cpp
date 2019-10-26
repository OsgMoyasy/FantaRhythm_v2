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
	msg = U"QR‚ğ‚©‚´‚µ‚Ä‚­‚¾‚³‚¢";
	readText = U"";
	webcam = Webcam(0);
}
QrRead::~QrRead(void) {
	TextureAsset::Unregister(U"qrreadback");
	TextureAsset::Unregister(U"qrreadmsg");
	FontAsset::Unregister(U"qrreadfont");
	isRead = false;
	
}
void QrRead::start(void) {
	
}
void QrRead::update(void) {
	if (!webcam.start()) {
		msg = U"WebƒJƒƒ‰‚ªÚ‘±‚³‚ê‚Ä‚¢‚Ü‚¹‚ñB";
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