#pragma once
#include <Siv3D.hpp>
#include <vector>

class AuraEffect {
private:
	Stopwatch stopwatch;
	String jobname;
	bool isDraw;
	int nowIndex;

	float prevS;


	std::vector<Texture> auraimage;

public:
	AuraEffect(String jobname);
	~AuraEffect();

	void setFlag(bool flag);

	void update();
	void draw(int x, int y);

};