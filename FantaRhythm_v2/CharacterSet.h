#pragma once
#include"FantaRhythm_v2.h"
#include"Character.h"
#include "Observer.h"

#define CHANUMBER 4
namespace JOB {
	enum LIST {
		SOLDIER,

	};
}


class CharacterSet:public Observer{
public:
	CharacterSet(int save[]);
	~CharacterSet();
	void draw();
	void update();
	void funcEvent(Obj obj);

private:

	class Character* cha[CHANUMBER];

};