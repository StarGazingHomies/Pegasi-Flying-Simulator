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

	StaticText(std::string name, std::string text, double x, double y, double size, glm::vec3 color);

	void draw() override;
	bool mouseEvent(MouseEvent mouseEvent) override;
	bool keyboardEvent(KeyEvent keyEvent) override;
	bool textEvent(unsigned int c) override;
};

class DynamicText: virtual public UIObject {
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
	float x1, y1, x2, y2;
	glm::vec3 color;
	glm::vec3 emptyColor;
	bool allowNewlines;
	bool active, selectionActive;
	float maxFontSize;
	int startPos = 0, endPos = 0;
	bool basis = false; // False for start, true for end
	float border;
	bool fitBox = true;
	float cursorThickness;
	std::vector<CharLinePos> charPositions;

	Texture backgroundTex;
	VAO backgroundVAO, cursorVAO;
	VBO backgroundVBO, cursorVBO;
	EBO backgroundEBO, cursorEBO;

	TextBox(std::string name, 
		std::string emptyText, 
		float x1, float y1, float x2, float y2, 
		const char* backgroundImg = nullptr, 
		glm::vec3 color = glm::vec3(0.0f), 
		glm::vec3 emptyColor = glm::vec3(0.5f), 
		float maxFontSize = 30, 
		bool allowNewlines = false, 
		float border = 2,
		float cursorThickness = 2);

	// Helper Methods
	bool isInRange(float mouseX, float mouseY);
	bool isValidAction(KeyEvent keyEvent);
	void write(std::string s);
	void backspace();
	int clamp(int pos);
	int nextWord(int direction);
	void move(int amount);
	void moveSelection(int amount);
	void moveSelectionAbs(int position);

	// Overrides
	void draw() override;
	bool mouseEvent(MouseEvent mouseEvent) override;
	bool keyboardEvent(KeyEvent keyEvent) override;
	bool textEvent(unsigned int c) override;
};
