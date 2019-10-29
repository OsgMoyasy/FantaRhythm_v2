#pragma once

class Scene {
public:
	Scene(void) {}
	virtual ~Scene(void) {}
	virtual bool isReady(void) = 0;
	virtual void start(void) = 0;
	virtual void update(void) = 0;
	virtual void draw(void) = 0;
};