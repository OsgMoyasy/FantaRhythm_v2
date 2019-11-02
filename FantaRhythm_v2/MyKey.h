#pragma once
#include <Siv3D.hpp>

namespace PSHBTN {
	constexpr int NONE = 0;
	constexpr int UP = 1;
	constexpr int DOWN = 2;
	constexpr int BOTH = 3;
}

class MyKey {
public:
	MyKey() = delete;
	static void isGamePad(void);
	static void getGameDownKey(int(&down)[4]);
	static void getGamePressKey(int(&press)[4]);
	static bool getDecisionKey(void);
	static bool getReturnKey(void);
	static bool getUpKey(void);
	static bool getDownKey(void);
	static bool getLeftKey(void);
	static bool getRightKey(void);
	static void setKeyLock(bool lock);
private:
	static bool lock;
	static bool padflag;
};