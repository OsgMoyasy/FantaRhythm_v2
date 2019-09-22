#pragma once
class Healer {
protected:
	int recoveryAmount;
public:
	virtual int isHeal(void) = 0;//‰ñ•œ‚ªo—ˆ‚é‚È‚ç‚Î‰ñ•œ—Ê‚ğ•Ô‚·
};