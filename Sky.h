#pragma once

#include"VAO.h"
#include"VBO.h"
#include"Shader.h"
#include"EBO.h"

#include<glm/glm.hpp>
#include<glm/gtc/constants.hpp>
#include<glm/gtc/type_ptr.hpp>

class Sky
{
public:
	VAO skyVAO;
	VBO skyVBO;
	EBO skyEBO;

	std::vector<float> vertices;
	std::vector<GLuint> indices;

	Sky();
	void Generate();
	void Draw(Shader& skyShader, glm::mat4 projMatrix, glm::mat4 viewMatrix);
	void Tick();
};

