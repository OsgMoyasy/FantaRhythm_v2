#pragma once
#include"FantaRhythm_v2.h"
#include"Character.h"

#define CHANUMBER 4

class CharacterSet {
public:
	CharacterSet(int save[]);
	~CharacterSet();
	void draw();
	void update();

private:
	class Character* cha[CHANUMBER];

};