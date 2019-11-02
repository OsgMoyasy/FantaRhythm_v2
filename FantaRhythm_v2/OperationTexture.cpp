#include"OperationTexture.h"

Texture* OperationTexture::decision;
Texture* OperationTexture::cancel;
Texture* OperationTexture::updown;
Texture* OperationTexture::leftright;
		 
MapFlip* OperationTexture::decision_flip;
MapFlip* OperationTexture::cancel_flip;
MapFlip* OperationTexture::updown_flip;
MapFlip* OperationTexture::leftright_flip;

Font* OperationTexture::font;

uint64 OperationTexture::switchTime;

void OperationTexture::initialize() {
	decision  = new Texture(U"resources/images/items/Operation/Decision.png");
	cancel    = new Texture(U"resources/images/items/Operation/Cancel.png");
	updown    = new Texture(U"resources/images/items/Operation/UpDown.png");
	leftright = new Texture(U"resources/images/items/Operation/LeftRight.png");

	decision_flip  = new MapFlip(*decision , TEXTURE_SIZE, TEXTURE_SIZE, true);
	cancel_flip    = new MapFlip(*cancel   , TEXTURE_SIZE, TEXTURE_SIZE, true);
	updown_flip    = new MapFlip(*updown   , TEXTURE_SIZE, TEXTURE_SIZE, true);
	leftright_flip = new MapFlip(*leftright, TEXTURE_SIZE, TEXTURE_SIZE, true);

	font = new Font(40, Typeface::Bold);
}
void OperationTexture::finalize() {
	delete decision;
	delete cancel;
	delete updown;
	delete leftright;

	delete decision_flip;
	delete cancel_flip;
	delete updown_flip;
	delete leftright_flip;
}
void OperationTexture::draw(Operation button, Vec2 drawpos, String string) {
	switch (button) {
	case DECISION:
		flipdraw(button, decision_flip, drawpos);
		break;
	case CANCEL:
		flipdraw(button, cancel_flip, drawpos);
		break;
	case UPDOWN:
		flipdraw(button, updown_flip, drawpos);
		break;
	case LEFTRIGHT:
		flipdraw(button, leftright_flip, drawpos);
		break;
	default:
		break;
	}

	(*font)(string).draw(drawpos + Vec2{ TEXTURE_SIZE + 3, 0 }, Palette::Black);
	(*font)(string).draw(drawpos + Vec2{ TEXTURE_SIZE, 0 });
}
void OperationTexture::flipdraw(Operation button, MapFlip* mapflip, Vec2 drawpos) {
	auto nowtime = Time::GetMillisec();
	if (switchTime <= nowtime) {
		switchTime = nowtime + TIME_BETWEEN;
		decision_flip->nextFlip();
		cancel_flip->nextFlip();
		updown_flip->nextFlip();
		leftright_flip->nextFlip();
	}

	mapflip->getFlip().draw(drawpos);
}
