#pragma once

#include "UIObject.h"
#include "../Graphics/Font.h"
#include "../resourceManager.h"
#include <GLFW/glfw3.h>
#include <functional>

class StaticText: virtual public UIObject {
public:
	std::string name;
	std::string text;
	double x, y;
	double fontSize;
	glm::vec3 color;

	StaticText(std::string text, double x, double y, double size, glm::vec3 color);

	void draw() override;
	bool mouseEvent(MouseEvent mouseEvent) override;
	bool keyboardEvent(KeyEvent keyEvent) override;
	bool textEvent(unsigned int c) override;
};

class DynamicText: virtual public UIObject{
public:
	std::string name;
	std::function<std::string()> text;
	double x, y;
	double fontSize;
	glm::vec3 color;

	DynamicText(std::function<std::string()> text, double x, double y, double size, glm::vec3 color);

	void draw() override;
	bool mouseEvent(MouseEvent mouseEvent) override;
	bool keyboardEvent(KeyEvent keyEvent) override;
	bool textEvent(unsigned int c) override;
};

class TextBox: virtual public UIObject {
public:
	std::string name;
	std::string text; // Just a string, O(n) is probably fine
	std::string emptyText;
	double x1, y1, x2, y2;
	glm::vec3 color;
	glm::vec3 emptyColor;
	bool allowNewlines;
	bool active;
	double maxFontSize = 12;
	int startPos = 0, endPos = 0;

	Texture backgroundTex;
	VAO textBoxVAO;
	VBO textBoxVBO;
	EBO textBoxEBO;

	TextBox(std::string name, std::string emptyText, double x1, double y1, double x2, double y2, glm::vec3 color = glm::vec3(1.0f), glm::vec3 emptyColor = glm::vec3(0.5f), double maxFontSize = 12, bool allowNewlines = false);

	bool isInRange(double mouseX, double mouseY);
	void write(std::string s);
	void backspace();

	void draw() override;
	bool mouseEvent(MouseEvent mouseEvent) override;
	bool keyboardEvent(KeyEvent keyEvent) override;
	bool textEvent(unsigned int c) override;
};
