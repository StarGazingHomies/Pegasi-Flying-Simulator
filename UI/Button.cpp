#include"Button.h"


Button::Button(std::string name, double x1, double y1, double x2, double y2, const char* defaultImg, const char* hoverImg, const char* pressedImg) {
	this->name = name;
	
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
		(float)x1, (float)y1, 0.0f, 1.0f,
		(float)x1, (float)y2, 0.0f, 0.0f,
		(float)x2, (float)y1, 1.0f, 1.0f,
		(float)x2, (float)y2, 1.0f, 0.0f,
		(float)x2, (float)y1, 1.0f, 1.0f,
		(float)x1, (float)y2, 0.0f, 0.0f,
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
	hoverFunc = [&]() {printf((this->name + " is hovered.\n").c_str()); };
	pressFunc = [&]() {printf((this->name + " is pressed.\n").c_str()); };
	releaseFunc = [&]() {printf((this->name + " is released.\n").c_str()); };

	dragFunc = [&](double x, double y) {
		printf("Dragged with delta %lf, %lf\n", x, y);
		this->x1 += x;
		this->x2 += x;
		this->y1 += y;
		this->y2 += y;
		printf("New coords: %lf, %lf, %lf, %lf\n", this->x1, this->y1, this->x2, this->y2);
		std::vector<float> vertices = {
			(float)this->x1, (float)this->y1, 0.0f, 1.0f,
			(float)this->x1, (float)this->y2, 0.0f, 0.0f,
			(float)this->x2, (float)this->y1, 1.0f, 1.0f,
			(float)this->x2, (float)this->y2, 1.0f, 0.0f,
			(float)this->x2, (float)this->y1, 1.0f, 1.0f,
			(float)this->x1, (float)this->y2, 0.0f, 0.0f,
		};
		this->buttonVBO.Data(vertices);
	};
}

// Functions to set callbacks
void Button::setHoverCallBack(std::function<void()> func) {
	this->hoverFunc = func;
}
void Button::setPressCallBack(const std::function<void()>& func) {
	this->pressFunc = func;
}
void Button::setReleaseCallBack(std::function<void()> func) {
	this->releaseFunc = func;
}
void Button::setDragCallBack(std::function<void(double, double)> func) {
	this->dragFunc = func;
}

bool Button::isInRange(double mouseX, double mouseY) {
	// Square bounds checking
	return x1 <= mouseX && mouseX <= x2 && y1 <= mouseY && mouseY <= y2;
}

bool Button::mouseEvent(double mouseX, double mouseY, int mouseButton) {
	// Note that in this implementation, going out of the button's bounds
	// while holding the button counts as releasing the button.
	// This may be different from a lot of common implementations.
	if (isInRange(mouseX, mouseY)) {
		if (mouseButton) {
			// Functions only execute once when it changes state
			if (buttonState != ButtonState::PRESS) {
				this->pressFunc();
				pressX = mouseX;
				pressY = mouseY;
			}
			else {
				dragFunc(mouseX - pressX, mouseY - pressY);
				pressX = mouseX;
				pressY = mouseY;
			}
			buttonState = ButtonState::PRESS;
			return true;
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
	return false;
}

bool Button::keyboardEvent(int key, int status)
{
	return false;
}

bool Button::textEvent(unsigned int c)
{
	return false;
}

void Button::draw() {
	Shader& buttonShader = resourceManager::getShader("button");

	buttonShader.Activate();
	glm::mat4 orthoProj = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f);
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