#pragma once
class Healer {
protected:
	int recoveryAmount;
public:
	virtual int isHeal(void) = 0;//�񕜂��o����Ȃ�Ή񕜗ʂ�Ԃ�
};