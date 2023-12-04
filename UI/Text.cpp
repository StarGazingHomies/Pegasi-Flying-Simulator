#include "Text.h"

// Consts for now
const float scrWidth = 800;
const float scrHeight = 600;

StaticText::StaticText(std::string text, double x, double y, double size, glm::vec3 color) {
	this->text = text;
	this->x = x;
	this->y = y;
	this->fontSize = size;
	this->color = color;
	this->name = "Static Text: " + text;
}

void StaticText::draw() {
	Font& f = resourceManager::getPrimaryFont();
	f.renderLine(text, DisplayPos{ Alignment::TOP_LEFT, (float)x, (float)y }, fontSize, color);
	f.renderAll(resourceManager::getShader("text"));
}

bool StaticText::mouseEvent(MouseEvent mouseEvent) {
	return false;
}

bool StaticText::keyboardEvent(KeyEvent keyEvent) {
	return false;
}

bool StaticText::textEvent(unsigned int c) {
	return false;
}

DynamicText::DynamicText(std::function<std::string()> text, double x, double y, double size, glm::vec3 color) {
	this->text = text;
	this->x = x;
	this->y = y;
	this->fontSize = size;
	this->color = color;
	this->name = "Dynamic Text";
}

void DynamicText::draw() {
	Font& f = resourceManager::getPrimaryFont();
	f.renderLine(text(), DisplayPos{Alignment::TOP_LEFT, (float)x, (float)y}, fontSize, color);
}

bool DynamicText::mouseEvent(MouseEvent mouseEvent) {
	return false;
}

bool DynamicText::keyboardEvent(KeyEvent keyEvent) {
	return false;
}

bool DynamicText::textEvent(unsigned int c) {
	return false;
}

TextBox::TextBox(std::string name, std::string emptyText, 
	float x1, float y1, float x2, float y2, 
	const char* backgroundImg, 
	glm::vec3 color, 
	glm::vec3 emptyColor, 
	float maxFontSize, 
	bool allowNewlines, 
	float border, 
	float cursorThickness) {

	this->name = name;
	this->emptyText = emptyText;

	this->x1 = std::min(x1, x2);
	this->y1 = std::min(y1, y2);
	this->x2 = std::max(x1, x2);
	this->y2 = std::max(y1, y2);
	this->color = color;
	this->emptyColor = emptyColor;
	this->allowNewlines = allowNewlines;
	this->maxFontSize = maxFontSize;
	this->border = border;
	this->cursorThickness = cursorThickness;

	this->startPos = 0;
	this->endPos = 0;
	this->text = "";

	this->active = false;

	// Background stuff
	std::vector<float> backgroundVertices = {
		this->x1, this->y1, 0.0f, 1.0f,
		this->x1, this->y2, 0.0f, 0.0f,
		this->x2, this->y1, 1.0f, 1.0f,
		this->x2, this->y2, 1.0f, 0.0f,
	};

	std::vector<unsigned int> indices = {
			0, 1, 2,
			3, 2, 1
	};

	backgroundVAO.Bind();
	backgroundVBO = VBO(backgroundVertices);
	backgroundEBO = EBO(indices);
	backgroundVAO.LinkAttrib(backgroundVBO, 0, 2, GL_FLOAT, 4 * sizeof(float), (void*)0);
	backgroundVAO.LinkAttrib(backgroundVBO, 1, 2, GL_FLOAT, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	backgroundEBO.Unbind();
	backgroundVBO.Unbind();
	backgroundVAO.Unbind();

	if (backgroundImg != nullptr) {
		backgroundTex = Texture{ backgroundImg, "diffuse", 0 };
	}
	else {
		backgroundTex = Texture{ "resources/white.png", "diffuse", 0 };
	}

	// Cursor stuff
	cursorVAO.Bind();
	std::vector<float> cursorVertices;
	cursorVBO = VBO(cursorVertices); // Empty VBO until data is subbed in
	cursorEBO = EBO(indices);
	cursorVAO.LinkAttrib(cursorVBO, 0, 2, GL_FLOAT, 5 * sizeof(float), (void*)0);
	cursorVAO.LinkAttrib(cursorVBO, 1, 3, GL_FLOAT, 5 * sizeof(float), (void*)(2 * sizeof(float)));
	cursorEBO.Unbind();
	cursorVBO.Unbind();
	cursorVAO.Unbind();

	// Finally, adjust borders
	this->x1 += border;
	this->y1 += border;
	this->x2 -= border;
	this->y2 -= border;
}

bool TextBox::isInRange(float mouseX, float mouseY) {
	return x1 <= mouseX && mouseX <= x2 && y1 <= mouseY && mouseY <= y2;
}

bool TextBox::isValidAction(KeyEvent keyEvent) {
	return keyEvent.action == GLFW_PRESS || keyEvent.action == GLFW_REPEAT;
}

void TextBox::write(std::string s) {
	printf("Writing: %s to [%d, %d]\n", s.c_str(), startPos, endPos);
	if (endPos != startPos) {
		text.replace(startPos, endPos - startPos, s);
	}
	else {
		text.insert(startPos, s);
	}
	startPos = endPos = startPos + 1;
}

void TextBox::backspace() {
	if (text.length() > 0) {
		if (startPos == endPos) {
			startPos--;
		}
		text.erase(startPos, endPos);
	}
	endPos = startPos;
}

void TextBox::draw() {
	// Drawing box around text
	Shader& buttonShader = resourceManager::getShader("texture");
	buttonShader.Activate();
	glm::mat4 orthoProj = glm::ortho(0.0f, scrWidth, scrHeight, 0.0f);
	glUniformMatrix4fv(glGetUniformLocation(buttonShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(orthoProj));

	backgroundVAO.Bind();
	backgroundEBO.Bind();
	backgroundTex.Bind();
	backgroundTex.texUnit(buttonShader, "diffuse", 0);

	// TODO: Only want to draw within the text box --> use stencil buffer
	//glStencilMask(0xFF);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// Drawing text
	Font& f = resourceManager::getPrimaryFont();
	float fontSize;

	if (fitBox) {
		float xSize = x2 - x1;
		float ySize = y2 - y1;
		std::pair<float, float> textSize = f.getTextSize(text, maxFontSize);

		float xScale = xSize / textSize.first;
		float yScale = ySize / textSize.second;
		fontSize = std::min(std::min(xScale, yScale), 1.0f) * maxFontSize;
	}
	else {
		fontSize = maxFontSize;
	}

	float xCenter = (x1 + x2) / 2;
	float yCenter = (y1 + y2) / 2;

	if (text != "") {
		f.renderLine(text, DisplayPos{ Alignment::CENTER, xCenter, yCenter }, fontSize, color);
	}
	else {
		f.renderLine(emptyText, DisplayPos{ Alignment::CENTER, xCenter, yCenter }, fontSize, emptyColor);
	}
	f.renderAll(resourceManager::getShader("text"));

	std::vector<CharLinePos> positions = f.getLinePos(text, DisplayPos{ Alignment::CENTER, xCenter, yCenter }, fontSize);
	// Draw cursor / selection
	if (active) {
		// For now, only draw when cursor is one thing
		// Also included is blinking cursor
		if (startPos == endPos && (int)(glfwGetTime() * 2) % 2 == 0) {
			float cursorx1{}, cursorx2{}, cursory1 = y1, cursory2 = y2;
			if (positions.size() == 0) {
				// Draw at center (need to depend on alignment)
				cursorx1 = cursorx2 = (x2 - x1) / 2;
			}
			else if (positions.size() <= endPos) {
				// Draw cursor at end of text
				cursorx1 = cursorx2 = positions[positions.size() - 1].x2;
			}
			else {
				// Draw cursor at end of character
				cursorx1 = cursorx2 = positions[endPos].x2;
			}
			cursorx1 += x1;
			cursorx2 += x1 + cursorThickness;

			std::vector<float> cursorVertices = {
				cursorx1, cursory1, 0.0f, 0.0f, 0.0f,
				cursorx1, cursory2, 0.0f, 0.0f, 0.0f,
				cursorx2, cursory1, 0.0f, 0.0f, 0.0f,
				cursorx2, cursory2, 0.0f, 0.0f, 0.0f,
			};

			// print vertices for debug
			//for (int i = 0; i < cursorVertices.size(); i++) {
			//	printf("%f ", cursorVertices[i]);
			//	if ((i + 1) % 5 == 0) printf("\n");
			//}

			// Draw cursor
			glDepthFunc(GL_ALWAYS);
			Shader& colorShader = resourceManager::getShader("color");
			colorShader.Activate();
			glUniformMatrix4fv(glGetUniformLocation(colorShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(orthoProj));

			cursorVBO.Data(cursorVertices);

			cursorVAO.Bind();
			cursorEBO.Bind();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		else {
			// TODO: Draw selection
		}

	}
}

bool TextBox::mouseEvent(MouseEvent mouseEvent) {

	double mouseX = mouseEvent.xPos;
	double mouseY = mouseEvent.yPos;
	int action = mouseEvent.action;
	int button = mouseEvent.button;
	if (active) {
		// Dragging
		// TODO: implement selection w/ dragging
	}
	// Ignore all pure movement events ?
	if (action == GLFW_PRESS) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && isInRange(mouseX, mouseY)) {
			// Pressing text box w/ left click
			// Don't know if right clicking should bring up menu or not... this is a game, not text editor
			active = true;

			// TODO: respect cursor position
			startPos = endPos = text.length();
			return true;
		}
		else {
			active = false;
			return false;
		}
	}

	return false;
}

bool TextBox::keyboardEvent(KeyEvent keyEvent) {
	// If active, handle keyboard events
	if (active) {
		// Enter, if it's enabled
		if (keyEvent.key == GLFW_KEY_ENTER && isValidAction(keyEvent)) {
			if (allowNewlines) write("\n");
			// TODO: do something (like confirm input) if enter is pressed and newlines are not allowed
		}
		// Backspace
		else if (keyEvent.key == GLFW_KEY_BACKSPACE && isValidAction(keyEvent)) {
			backspace();
		}
		printf("Key: %d, New Text: %s\n---\n", keyEvent.key, text.c_str());
	}
	return active;
}

bool TextBox::textEvent(unsigned int c) {
	// If active, handle text events
	if (active) {
		if (c >= 32 && c <= 126) {
			write(std::string(1, c));
		}
		printf("Char: %d, New Text: %s\n---\n", c, text.c_str());
	}
	return active;
}

