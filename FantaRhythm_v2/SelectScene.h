#pragma once
#include <vector>
class SelectScene{
public:
	virtual~SelectScene() {};
	virtual void update(int& rotation, int elementCount) = 0;
	virtual void getElementName(std::vector<String>& elementName, int elementCount) = 0;
	virtual int getCursor(void) = 0;
};

