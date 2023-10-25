#pragma once

#include<functional>

#include"../Graphics/Texture.h"
#include"../Graphics/Shader.h"
#include"../Graphics/VAO.h"
#include"../Graphics/VBO.h"
#include"../Graphics/EBO.h"

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

enum class ButtonState {
	DEFAULT,
	HOVER,
	PRESS
};

class Button {
public:
	int x1, x2, y1, y2;
	int pressX, pressY;
	// Callback (when the button is ticked)
	std::function<void()> hoverFunc, pressFunc, releaseFunc;
	// Dragging function takes to arguments, deltaX and deltaY
	std::function<void(int, int)> dragFunc;
	// Render
	Texture defaultTex, hoverTex, pressedTex;
	VAO buttonVAO;
	VBO buttonVBO;
	EBO buttonEBO;
	ButtonState buttonState;

	// Constructors
	Button();

	void set(int x1, int y1, int x2, int y2, const char* defaultImg = nullptr, const char* hoverImg = nullptr, const char* pressedImg = nullptr);
	
	// Callbacks
	void setHoverCallBack(std::function<void()> func);
	void setPressCallBack(const std::function<void()>& func);
	void setReleaseCallBack(std::function<void()> func);
	void setDragCallBack(std::function<void(int, int)> func);

	bool isInRange(int mouseX, int mouseY);
	// buttonStatus will be 0 if not pressed and 1 if pressed
	void Tick(int mouseX, int mouseY, int buttonStatus);
	void Draw(Shader& buttonShader);

	// Deconstructors... not used right now
	void Delete();
};