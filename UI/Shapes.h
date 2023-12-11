#pragma once

#include "UIObject.h"
#include "../resourceManager.h"

class TexturedRectangle : virtual public UIObject {

public:
	std::string texture;
	RectAlignment pos;
	VAO vao;
	VBO vbo;
	EBO ebo;

	TexturedRectangle(std::string texture, glm::vec2 pos1, glm::vec2 pos2);
	TexturedRectangle(std::string texture, RectAlignment pos);

	void setTexture(std::string texture);
	void setPosition(glm::vec2 pos1, glm::vec2 pos2, glm::vec2 alignment = glm::vec2(0.0f));
	void setPosition(RectAlignment pos);

	void draw();
	bool mouseEvent(MouseEvent mouseEvent) { return false;}
	bool keyboardEvent(KeyEvent keyEvent) { return false;}
	bool textEvent(unsigned int c) { return false;}
};
