#pragma once

#include <string>
#include <glm/glm.hpp>

class UIObject {
public:
	std::string name;

	virtual void draw() = 0;
	// buttonStatus will be 0 if not pressed and 1 if pressed
	virtual bool mouseEvent(double mouseX, double mouseY, int buttonStatus) = 0;
	virtual bool keyboardEvent(int key, int status) = 0;
	virtual bool textEvent(unsigned int c) = 0;
};

