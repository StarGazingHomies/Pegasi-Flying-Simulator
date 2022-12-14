#pragma once

#include"Phys_Object.h"

class Physics {
public:
	std::vector<OBB> objects;

	Physics();
	void Draw(Shader& shader, glm::mat4 projMatrix, glm::mat4 viewMatrix);
	void Tick(double time);
};