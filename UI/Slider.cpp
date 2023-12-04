#include "Slider.h"

Slider::Slider(std::string name, int x1, int y1, int x2, int y2) {
	this->name = name;
	this->x1 = x1;
	this->x2 = x2;
	this->y1 = y1;
	this->y2 = y2;
	this->relativeValue = 0;

	float currentX = x1 + relativeValue * (x2 - x1);
	
	std::vector<float> vertices = {
		(float)x1, (float)y1, 0.2f, 0.2f, 0.2f,
		(float)x2, (float)y1, 0.2f, 0.2f, 0.2f,
		(float)x1, (float)y2, 0.2f, 0.2f, 0.2f,
		(float)x2, (float)y2, 0.2f, 0.2f, 0.2f,  // Bounding box

		(float)currentX - halfThickness, (float)y1, 0.8f, 0.8f, 0.8f,
		(float)currentX + halfThickness, (float)y1, 0.8f, 0.8f, 0.8f,
		(float)currentX - halfThickness, (float)y2, 0.8f, 0.8f, 0.8f,
		(float)currentX + halfThickness, (float)y2, 0.8f, 0.8f, 0.8f,  // Slider
	};

	std::vector<unsigned int> indices = {
		0, 1, 2,
		3, 2, 1,  // Bounding box

		4, 5, 6,
		6, 7, 5,  // Slider
	};

	this->sliderVAO.Bind();

	this->sliderVBO = VBO{ vertices, GL_DYNAMIC_DRAW };
	this->sliderEBO = EBO{ indices };

	this->sliderVAO.LinkAttrib(this->sliderVBO, 0, 2, GL_FLOAT, 5 * sizeof(float), (void*)0);
	this->sliderVAO.LinkAttrib(this->sliderVBO, 1, 3, GL_FLOAT, 5 * sizeof(float), (void*)(2 * sizeof(float)));
	this->sliderVAO.Unbind();
}

float Slider::getMouseValue(float mousePos) {
	float val = (mousePos - x1) / (x2 - x1);
	if (val < 0) val = 0;
	if (val > 1) val = 1;
	return val;
}

void Slider::draw() {
	Shader& sliderShader = resourceManager::getShader("2DColor");
	sliderShader.Activate();

	glDepthFunc(GL_LEQUAL);

	glm::mat4 orthoProj = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f);
	glUniformMatrix4fv(glGetUniformLocation(sliderShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(orthoProj));

	this->sliderVAO.Bind();
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
	this->sliderVAO.Unbind();
}

bool Slider::mouseEvent(MouseEvent mouseEvent) {
	double x = mouseEvent.xPos;
	double y = mouseEvent.yPos;
	int button = mouseEvent.button;
	int action = mouseEvent.action;
	int mods = mouseEvent.mods;

	if (mouseEvent.type) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			if (x >= x1 && x <= x2 && y >= y1 && y <= y2) {
				relativeValue = getMouseValue(x);
				float currentX = x1 + relativeValue * (x2 - x1);

				std::vector<float> vertices = {
					(float)x1, (float)y1, 0.2f, 0.2f, 0.2f,
					(float)x2, (float)y1, 0.2f, 0.2f, 0.2f,
					(float)x1, (float)y2, 0.2f, 0.2f, 0.2f,
					(float)x2, (float)y2, 0.2f, 0.2f, 0.2f,  // Bounding box

					(float)currentX - halfThickness, (float)y1, 0.8f, 0.8f, 0.8f,
					(float)currentX + halfThickness, (float)y1, 0.8f, 0.8f, 0.8f,
					(float)currentX - halfThickness, (float)y2, 0.8f, 0.8f, 0.8f,
					(float)currentX + halfThickness, (float)y2, 0.8f, 0.8f, 0.8f,  // Slider
				};

				this->sliderVBO.Data(vertices);
				pressed = true;
				return true;
			}
		}
		else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
			pressed = false;
		}
	}
	else {
		if (!pressed) return false;
		relativeValue = getMouseValue(x);
		float currentX = x1 + relativeValue * (x2 - x1);

		std::vector<float> vertices = {
			(float)x1, (float)y1, 0.2f, 0.2f, 0.2f,
			(float)x2, (float)y1, 0.2f, 0.2f, 0.2f,
			(float)x1, (float)y2, 0.2f, 0.2f, 0.2f,
			(float)x2, (float)y2, 0.2f, 0.2f, 0.2f,  // Bounding box

			(float)currentX - halfThickness, (float)y1, 0.8f, 0.8f, 0.8f,
			(float)currentX + halfThickness, (float)y1, 0.8f, 0.8f, 0.8f,
			(float)currentX - halfThickness, (float)y2, 0.8f, 0.8f, 0.8f,
			(float)currentX + halfThickness, (float)y2, 0.8f, 0.8f, 0.8f,  // Slider
		};

		this->sliderVBO.Data(vertices);
		return true;
	}
	return false;
}

bool Slider::keyboardEvent(KeyEvent keyEvent) {
	return false;
}

bool Slider::textEvent(unsigned int c) {
	return false;
}
