#pragma once

#include <functional>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
		 
#include "../Graphics/Texture.h"
#include "../Graphics/Shader.h"
#include "../Graphics/VAO.h"
#include "../Graphics/VBO.h"
#include "../Graphics/EBO.h"
#include "../resourceManager.h"
#include "UIObject.h"


enum class ButtonState {
	DEFAULT,
	HOVER,
	PRESS
};

class Button : virtual public UIObject {
public:
	std::string name = "Default Button Name";

	double x1, x2, y1, y2;
	double pressX, pressY;
	// Callback (when the button is ticked)
	std::function<void()> hoverFunc, pressFunc, releaseFunc;
	// Dragging function takes to arguments, deltaX and deltaY
	std::function<void(double, double)> dragFunc;
	// Render
	Texture defaultTex, hoverTex, pressedTex;
	VAO buttonVAO;
	VBO buttonVBO;
	EBO buttonEBO;
	ButtonState buttonState;

	// Constructors
	Button(std::string name, double x1, double y1, double x2, double y2, const char* defaultImg = nullptr, const char* hoverImg = nullptr, const char* pressedImg = nullptr);
	
	// Callbacks
	void setHoverCallBack(std::function<void()> func);
	void setPressCallBack(const std::function<void()>& func);
	void setReleaseCallBack(std::function<void()> func);
	void setDragCallBack(std::function<void(double, double)> func);

	bool isInRange(double mouseX, double mouseY);

	// Callbacks
	void draw() override;
	bool mouseEvent(MouseEvent mouseEvent) override;
	bool keyboardEvent(KeyEvent keyEvent) override;
	bool textEvent(unsigned int c) override;

	// Deconstructors... not used right now
	void Delete();
};