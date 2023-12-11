#pragma once

#include "UIObject.h"
#include "../Graphics/Texture.h"
#include "../Graphics/Shader.h"
#include "../Graphics/VAO.h"
#include "../Graphics/VBO.h"
#include "../Graphics/EBO.h"
#include "../resourceManager.h"

#include <GLFW/glfw3.h>

class Slider : virtual public UIObject {
public:
	int x1, x2, y1, y2, halfThickness = 5;
	float relativeValue;
	VAO sliderVAO;
	VBO sliderVBO;
	EBO sliderEBO;
	std::string name;
	bool pressed = false;
	
	Slider(std::string name, int x1, int y1, int x2, int y2);

	float posToValue(float mousePos);
	float valueToPos(float value);

	void draw();
	bool mouseEvent(MouseEvent mouseEvent);
	bool keyboardEvent(KeyEvent keyEvent);
	bool textEvent(unsigned int c);
};