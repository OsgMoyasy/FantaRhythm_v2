#pragma once

enum KeySound{
	DECISION,
	RETURN,
	UP,
	DOWN,
};

#include <Siv3D.hpp>
#include"GameEffect.h"

class MyKeySound {
public:
	static void playSE(KeySound key) {
		switch (key) {
		case DECISION:
			decision->play();
			break;
		case RETURN:
			ret->play();
			break;
		case UP:
			up->play();
			break;
		case DOWN:
			down->play();
			break;
		default:
			break;
		}
	}
	static SE* decision;
	static SE* ret;
	static SE* up;
	static SE* down;
};


/*
class MyKeySound {
public:
	MyKeySound(
		const FilePath& _decision,
		const FilePath& _ret,
		const FilePath& _up,
		const FilePath& _down
	) {
		decision = new SE(_decision);
		ret = new SE(_ret);
		up = new SE(_up);
		down = new SE(_down);
	}
	static void playSE(KeySound key) {
		switch (key) {
		case DECISION:
			decision->play();
			break;
		case RETURN:
			ret->play();
			break;
		case UP:
			up->play();
			break;
		case DOWN:
			down->play();
			break;
		default:
			break;
		}
	}
private:
	static SE* decision;
	static SE* ret;
	static SE* up;
	static SE* down;
};
*/