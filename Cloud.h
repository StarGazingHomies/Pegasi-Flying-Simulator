#pragma once

#include<random>

#include"VAO.h"
#include"VBO.h"
#include"Shader.h"
#include"EBO.h"

#include<glm/glm.hpp>
#include<glm/gtc/constants.hpp>
#include<glm/gtc/type_ptr.hpp>

struct CloudNode {
	glm::vec3 pos;
	float radius;
	int depth;
};

class Cloud
{
	// A cloud!
	std::vector<CloudNode> nodes;

	Cloud();
	void Generate(int branchNum, float initialRadius, float radiusDecay, int numOfIterations = 3);
};

