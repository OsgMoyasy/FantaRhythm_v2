#pragma once
#include"OperationDefine.h"
#include"GameEffect.h"
#include<Siv3D.hpp>

class OperationTexture {
public:
	static void initialize();
	static void finalize();
	static void draw(Operation button, Vec2 drawpos, String string);
private:
	static void flipdraw(Operation button, MapFlip* mapflip, Vec2 drawpos);

	static Texture* decision;
	static Texture* cancel;
	static Texture* updown;
	static Texture* leftright;

	static MapFlip* decision_flip;
	static MapFlip* cancel_flip;
	static MapFlip* updown_flip;
	static MapFlip* leftright_flip;
	
	static Font* font;

	static uint64 switchTime;
	static constexpr int TIME_BETWEEN = 1000;
	static constexpr int TEXTURE_SIZE = 50;
};
