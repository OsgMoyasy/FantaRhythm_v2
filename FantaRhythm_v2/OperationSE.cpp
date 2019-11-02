#include"OperationSE.h"

SE* OperationSE::decision;
SE* OperationSE::cancel;
SE* OperationSE::lever;

void OperationSE::initialize() {
	decision = new SE(U"resources/musics/effects/decision.mp3");
	cancel   = new SE(U"resources/musics/effects/1nort.mp3");
	lever    = new SE(U"resources/musics/effects/cancel.mp3");
}
void OperationSE::finalize() {
	delete decision;
	delete cancel;
	delete lever;
}
void OperationSE::play(Operation button) {
	switch (button) {
	case DECISION:
		decision->play();
		break;
	case CANCEL:
		cancel->play();
		break;
	case LEVER:
		lever->play();
		break;
	default:
		break;
	}
}
