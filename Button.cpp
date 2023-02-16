#include"Button.h"

Button::Button(int x1, int y1, int x2, int y2, const char* defaultImg, const char* hoverImg, const char* pressedImg) {
	// Sides
	this->x1 = x1;
	this->x2 = x2;
	this->y1 = y1;
	this->y2 = y2;

	if (defaultImg != nullptr) {
		defaultTex = Texture(defaultImg, "diffuse", 0);
	}
	if (hoverImg != nullptr) {
		hoverTex = Texture(hoverImg, "diffuse", 0);
	}
	if (pressedImg != nullptr) {
		pressedTex = Texture(pressedImg, "diffuse", 0);
	}


	std::vector<float> vertices = {
		(float)x1, (float)y1, 0.0f, 0.0f,
		(float)x1, (float)y2, 0.0f, 1.0f,
		(float)x2, (float)y1, 1.0f, 0.0f,
		(float)x2, (float)y2, 1.0f, 1.0f,
		(float)x2, (float)y1, 1.0f, 0.0f,
		(float)x1, (float)y2, 0.0f, 1.0f,
	};

	buttonVAO.Bind();
	buttonVBO = VBO(vertices);
	buttonVAO.LinkAttrib(buttonVBO, 0, 2, GL_FLOAT, 4 * sizeof(float), (void*)0);
	buttonVAO.LinkAttrib(buttonVBO, 1, 2, GL_FLOAT, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	std::vector<GLuint> indices = {
		0,1,2,3,2,1
	};

	buttonEBO = EBO(indices);

	// Default functions, mostly used when debugging
	std::string buttonRepString = "Button at <" + \
		std::to_string(x1) + ", " + std::to_string(y1) + "> to <" \
		+ std::to_string(x2) + ", " + std::to_string(y2) + ">";
	hoverFunc = [buttonRepString]() {printf((buttonRepString + " is hovered.\n").c_str()); };
	pressFunc = [buttonRepString]() {printf((buttonRepString + " is pressed.\n").c_str()); };
	releaseFunc = [buttonRepString]() {printf((buttonRepString + " is released.\n").c_str()); };
	dragFunc = [](int x, int y) {};
	/* Fix this later
	dragFunc = [&](int x, int y) {
		printf("Dragged with delta %d, %d\n", x, y);
		printf("New coords: %d, %d, %d, %d\n", x1, y1, x2, y2);
		std::vector<float> vertices = {
		(float)x1, (float)y1, 0.0f, 0.0f,
		(float)x1, (float)y2, 0.0f, 1.0f,
		(float)x2, (float)y1, 1.0f, 0.0f,
		(float)x2, (float)y2, 1.0f, 1.0f,
		(float)x2, (float)y1, 1.0f, 0.0f,
		(float)x1, (float)y2, 0.0f, 1.0f,
		};
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size()*sizeof(float), vertices.data());
	};*/
}

// Functions to set callbacks
void Button::setHoverCallBack(std::function<void()> func) {
	hoverFunc = func;
}
void Button::setPressCallBack(std::function<void()> func) {
	pressFunc = func;
}
void Button::setReleaseCallBack(std::function<void()> func) {
	releaseFunc = func;
}
void Button::setDragCallBack(std::function<void(int, int)> func) {
	dragFunc = func;
}

bool Button::isInRange(int mouseX, int mouseY) {
	// Square bounds checking
	return x1 <= mouseX && mouseX <= x2 && y1 <= mouseY && mouseY <= y2;
}

void Button::Tick(int mouseX, int mouseY, int mouseButton) {
	// Note that in this implementation, going out of the button's bounds
	// while holding the button counts as releasing the button.
	// This may be different from a lot of common implementations.
	if (isInRange(mouseX, mouseY)) {
		if (mouseButton) {
			// Functions only execute once when it changes state
			if (buttonState != ButtonState::PRESS) {
				pressFunc();
				pressX = mouseX;
				pressY = mouseY;
			}
			else {
				dragFunc(mouseX - pressX, mouseY - pressY);
				pressX = mouseX;
				pressY = mouseY;
			}
			buttonState = ButtonState::PRESS;
		}
		else {
			// If the button wasn't interacted with at all
			if (buttonState == ButtonState::DEFAULT) hoverFunc();
			// If the button was just pressed
			else if (buttonState == ButtonState::PRESS) releaseFunc();
			buttonState = ButtonState::HOVER;
		}
	}
	else {
		// Just pressed the button
		if (buttonState == ButtonState::PRESS) releaseFunc();
		buttonState = ButtonState::DEFAULT;
	}
}

void Button::Draw(Shader& buttonShader) {

	buttonShader.Activate();
	glm::mat4 orthoProj = glm::ortho(0.0f, 800.0f, 0.0f, 800.0f);
	glUniformMatrix4fv(glGetUniformLocation(buttonShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(orthoProj));

	switch (buttonState) {
	case ButtonState::DEFAULT:
		defaultTex.Bind(); 
		defaultTex.texUnit(buttonShader, "diffuse", 0);
		break;
	case ButtonState::HOVER:
		hoverTex.Bind();
		hoverTex.texUnit(buttonShader, "diffuse", 0);
		break;
	case ButtonState::PRESS:
		pressedTex.Bind();
		pressedTex.texUnit(buttonShader, "diffuse", 0);
		break;
	}

	buttonVAO.Bind();
	buttonVBO.Bind();
	buttonEBO.Bind();

	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Button::Delete() {
	buttonVAO.Delete();
	buttonVBO.Delete();
	buttonEBO.Delete();
}