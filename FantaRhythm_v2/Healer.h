#pragma once
class Healer {
protected:
	int recoveryAmount;
public:
	virtual int isHeal(void) = 0;//回復が出来るならば回復量を返す
};