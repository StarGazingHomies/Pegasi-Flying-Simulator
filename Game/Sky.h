#pragma once

#include"../Graphics/VAO.h"
#include"../Graphics/VBO.h"
#include"../Graphics/Shader.h"
#include"../Graphics/EBO.h"
#include"../Graphics/Texture.h"

#include<glm/glm.hpp>
#include<glm/gtc/constants.hpp>
#include<glm/gtc/type_ptr.hpp>

class Sky
{
public:
	VAO skyVAO;
	VBO skyVBO;
	EBO skyEBO;
	Texture sunTex, moonTex;

	float time = 1000;

	std::vector<float> vertices;
	std::vector<GLuint> indices;

	glm::vec3 skyColourFromTime();
	glm::vec3 sunPosFromTime();

	Sky();
	void Generate();
	void Draw(const Shader& skyShader, glm::mat4 projMatrix, glm::mat4 viewMatrix);
	void Tick();
};

