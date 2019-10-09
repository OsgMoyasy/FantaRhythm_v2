#include "MyKey.h"
bool MyKey::lock = false;
bool MyKey::padflag;
void MyKey::isGamePad() {
	if (Gamepad(0).isConnected()) {
		padflag = true;
	}
	else {
		padflag = false;
	}
}

void MyKey::getGameDownKey(int(&down)[4]) {
	if (!lock) {
		if (padflag) {
			//上ボタン
			down[0] = Gamepad(0).buttons[0].down() ? PSHBTN::UP : 0;
			down[1] = Gamepad(0).buttons[1].down() ? PSHBTN::UP : 0;
			down[2] = Gamepad(0).buttons[2].down() ? PSHBTN::UP : 0;
			down[3] = Gamepad(0).buttons[3].down() ? PSHBTN::UP : 0;
			//下ボタン
			down[0] += Gamepad(0).buttons[4].down() ? PSHBTN::DOWN : 0;
			down[1] += Gamepad(0).buttons[5].down() ? PSHBTN::DOWN : 0;
			down[2] += Gamepad(0).buttons[6].down() ? PSHBTN::DOWN : 0;
			down[3] += Gamepad(0).buttons[7].down() ? PSHBTN::DOWN : 0;
		}
		else {//キーボード
			//上ボタン
			down[0] = KeyQ.down() ? PSHBTN::UP : 0;
			down[1] = KeyW.down() ? PSHBTN::UP : 0;
			down[2] = KeyE.down() ? PSHBTN::UP : 0;
			down[3] = KeyR.down() ? PSHBTN::UP : 0;
			//下ボタン
			down[0] += KeyA.down() ? PSHBTN::DOWN : 0;
			down[1] += KeyS.down() ? PSHBTN::DOWN : 0;
			down[2] += KeyD.down() ? PSHBTN::DOWN : 0;
			down[3] += KeyF.down() ? PSHBTN::DOWN : 0;
		}
	}
	else {
		down[0] = 0;
		down[1] = 0;
		down[2] = 0;
		down[3] = 0;
	}
}

void MyKey::getGamePressKey(int(&press)[4]) {
	if (!lock) {
		if (padflag) {
			//上ボタン
			press[0] = Gamepad(0).buttons[0].pressed() ? PSHBTN::UP : 0;
			press[1] = Gamepad(0).buttons[1].pressed() ? PSHBTN::UP : 0;
			press[2] = Gamepad(0).buttons[2].pressed() ? PSHBTN::UP : 0;
			press[3] = Gamepad(0).buttons[3].pressed() ? PSHBTN::UP : 0;
			//下ボタン
			press[0] += Gamepad(0).buttons[4].pressed() ? PSHBTN::DOWN : 0;
			press[1] += Gamepad(0).buttons[5].pressed() ? PSHBTN::DOWN : 0;
			press[2] += Gamepad(0).buttons[6].pressed() ? PSHBTN::DOWN : 0;
			press[3] += Gamepad(0).buttons[7].pressed() ? PSHBTN::DOWN : 0;
		}
		else {//キーボード
			//上ボタン
			press[0] = KeyQ.pressed() ? PSHBTN::UP : 0;
			press[1] = KeyW.pressed() ? PSHBTN::UP : 0;
			press[2] = KeyE.pressed() ? PSHBTN::UP : 0;
			press[3] = KeyR.pressed() ? PSHBTN::UP : 0;
			//下ボタン
			press[0] += KeyA.pressed() ? PSHBTN::DOWN : 0;
			press[1] += KeyS.pressed() ? PSHBTN::DOWN : 0;
			press[2] += KeyD.pressed() ? PSHBTN::DOWN : 0;
			press[3] += KeyF.pressed() ? PSHBTN::DOWN : 0;
		}
	}
	else {
		press[0] = 0;
		press[1] = 0;
		press[2] = 0;
		press[3] = 0;
	}
}

bool MyKey::getDecisionKey(void) {
	if (!lock) {
		if (padflag) {

		}
		else {
			return KeyEnter.down();
		}
	}
	else {
		return false;
	}
}
bool MyKey::getReturnKey(void) {
	if (!lock) {
		if (padflag) {

		}
		else {
			return KeyBackspace.down();
		}
	}
	else {
		return false;
	}
}
bool MyKey::getUpKey(void) {
	if (!lock) {
		if (padflag) {

		}
		else {
			return KeyUp.pressed();
		}
	}
	else {
		return false;
	}
}
bool MyKey::getDownKey(void) {
	if (!lock) {
		if (padflag) {

		}
		else {
			return KeyDown.pressed();
		}
	}
	else {
		return false;
	}
}

void MyKey::setKeyLock(bool lock) {
	MyKey::lock = lock;
}