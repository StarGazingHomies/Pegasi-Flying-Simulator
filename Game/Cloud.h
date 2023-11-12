#pragma once

#include<random>

#include"../Graphics/VAO.h"
#include"../Graphics/VBO.h"
#include"../Graphics/Shader.h"
#include"../Graphics/EBO.h"
#include "Noise.h"

#include<glm/glm.hpp>
#include<glm/gtc/constants.hpp>
#include<glm/gtc/type_ptr.hpp>

struct CloudNode {
	glm::vec3 pos;
	float radius;
};

class Cloud {
public:
	// The spheres that make up a cloud
	std::vector<CloudNode> nodes;
	glm::vec3 pos;

	Cloud(glm::vec3 pos, int branchNum, float initialRadius, float radiusDecay, int numOfIterations = 3);
};

class Clouds
{
public:
	std::vector<Cloud> cloudList;

	VAO sphereVAO;
	VBO sphereVBO;
	VBO cloudsInstancedVBO;
	EBO sphereEBO;

	std::vector<float> vertices;
	std::vector<GLuint> indices;
	std::vector<float> instanceData;

	Clouds();
	void Generate(int amount=1, int branchNum=4, float initialRadius=3.5f, float radiusDecay=0.75f, int numOfIterations = 4);
	void Draw(Shader& cloudShader, glm::mat4 proj, glm::mat4 view);
};
