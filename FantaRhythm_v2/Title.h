#pragma once
#include "FantaRhythm_v2.h"
class Title{
public:
	Title(void);
	~Title(void);
	int update(void);
	void draw(void);
private:
	double alpha;
	void changeAlpha(void);
};
