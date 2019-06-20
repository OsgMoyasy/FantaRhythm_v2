#pragma once

class Scene {
public:
	Scene(void) {}
	virtual ~Scene(void) {}
	virtual int update(void) = 0;
	virtual void draw(void) = 0;
};