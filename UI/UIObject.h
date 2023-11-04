#pragma once

#include <string>
#include <glm/glm.hpp>
#include "Events.h"

class UIObject {
public:
	std::string name;

	virtual void draw() = 0;
	// buttonStatus will be 0 if not pressed and 1 if pressed
	virtual bool mouseEvent(MouseEvent mouseEvent) = 0;
	virtual bool keyboardEvent(KeyEvent keyEvent) = 0;
	virtual bool textEvent(unsigned int c) = 0;
};

