#pragma once
#include"OperationDefine.h"
#include"GameEffect.h"

class OperationSE {
public:
	static void initialize();
	static void finalize();
	static void play(Operation button);
private:
	static SE* decision;
	static SE* cancel;
	static SE* lever;
};
