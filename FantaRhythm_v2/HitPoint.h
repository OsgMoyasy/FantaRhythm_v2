#pragma once
#include "Observer.h"
#include "FantaRhythm_v2.h"

class HitPoint :public Observer{
private:
	int hp;
	void funcEvent(Obj o);
	
public:
	HitPoint();
	~HitPoint();
	
	void draw();
	void update();
};