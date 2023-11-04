#include "Text.h"

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

TextBox::TextBox(std::string name, std::string emptyText, double x1, double y1, double x2, double y2, glm::vec3 color, glm::vec3 emptyColor, double maxFontSize, bool allowNewlines) {
	this->name = name;
	this->emptyText = emptyText;
	this->x1 = x1;
	this->y1 = y1;
	this->x2 = x2;
	this->y2 = y2;
	this->color = color;
	this->emptyColor = emptyColor;
	this->allowNewlines = allowNewlines;
	this->maxFontSize = maxFontSize;

	this->startPos = 0;
	this->endPos = 0;
	this->text = "";
	this->active = false;
}

bool TextBox::isInRange(double mouseX, double mouseY) {
	return (mouseX >= x1 && mouseX <= x2 && mouseY >= y1 && mouseY <= y2);
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
	Font& f = resourceManager::getPrimaryFont();

	if (text != "") {
		f.renderLine(text, DisplayPos{ Alignment::TOP_LEFT, (float)x1, (float)y1 }, maxFontSize, color);
	}
	else {
		f.renderLine(emptyText, DisplayPos{ Alignment::TOP_LEFT, (float)x1, (float)y1 }, maxFontSize, emptyColor);
	}
}

bool TextBox::mouseEvent(MouseEvent mouseEvent) {
	//if (isInRange(mouseX, mouseY)) {
	//	if (buttonStatus == GLFW_PRESS) {
	//		active = true;
	//		// Get the location of the mouse button press, relative to text...
	//		// (not right now...)
	//		startPos = text.length();
	//		endPos = text.length();
	//		return true;
	//	}
	//}
	//else {
	//	active = false;
	//}
	return false;
}

bool TextBox::keyboardEvent(KeyEvent keyEvent) {
	// If active, handle keyboard events
	if (active) {
		// Enter, if it's enabled
		if (keyEvent.key == GLFW_KEY_ENTER && keyEvent.action == GLFW_PRESS && allowNewlines) {
			write("\n");
		}
		// Backspace
		else if (keyEvent.key == GLFW_KEY_BACKSPACE && keyEvent.action == GLFW_PRESS) {
			backspace();
		}
	}

	printf("Key: %d, New Text: %s\n---\n", keyEvent.key, text.c_str());

	return active;
}

bool TextBox::textEvent(unsigned int c) {
	// If active, handle text events
	if (active) {
		if (c >= 32 && c <= 126) {
			write(std::string(1, c));
		}
	}
	printf("Char: %d, New Text: %s\n---\n", c, text.c_str());
	return active;
}

